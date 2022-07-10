from models.layers import conv_bn_act, Flatten,SamePadConv2d,SEModule,DropConnect
import torch
import torch.nn as nn 
import math


class MBConv(nn.Module):
    def __init__(self, in_ , out_ ,expand,
                    kernel_size,stride,skip,
                    se_ratio,dc_ratio=0.2):
        super().__init__()
        mid_= in_ * expand
        self.expand_conv = conv_bn_act(in_,mid_,kernel_size=1,bias=False) if expand!=1 else nn.Identity()
        
        self.depth_wise_conv = conv_bn_act(mid_,mid_,
                                            kernel_size=kernel_size,
                                            stride=stride,
                                            groups=mid_,bias=False)
        
        self.se = SEModule(mid_,int(in_*se_ratio)) if se_ratio>0 else nn.Identity()
        
        self.project_conv=nn.Sequential(
            SamePadConv2d(mid_,out_,kernel_size=1,stride=1,bias=False),
            nn.BatchNorm2d(out_)
        )

        self.skip= skip and (stride==1) and(in_ == out_)
        self.drop_connect=nn.Identity()

    def forward(self,inputs):
        expand=self.expand_conv(inputs)
        x=self.depth_wise_conv(expand)
        x=self.se(x)
        x=self.project_conv(x)
        if self.skip:
            x=self.drop_connect(x)
            x=x+inputs
        return x

class MBBlock(nn.Module):
    def __init__(self,in_,out_,expand,kernel,stride,num_repeat,skip, se_ratio, drop_connect_ratio=0.2):
        super().__init__()
        layers=[MBConv(in_,out_,expand, kernel , stride , skip,se_ratio,drop_connect_ratio)]
        for i in range(1,num_repeat):
            layers.append(MBConv(out_,out_,expand, kernel,1,skip,se_ratio,drop_connect_ratio))
        self.layers = nn.Sequential(*layers)
    
    def forward(self,x):
        return self.layers(x)

class EfficientNet(nn.Module):
    def __init__(self,width_coeff,depth_coeff,
                depth_div=8,min_depth=None,max_depth=None,
                dropout_rate=0.2,drop_connect_rate=0.2,num_classes=1000):
        super().__init__()
        min_depth=min_depth or depth_div

        def scaled_width(x):
            if not width_coeff:
                return x

            x*= width_coeff
            new_x=max(min_depth,int(x+depth_div/2)//depth_div*depth_div)
            if new_x <0.9*x:
                new_x +=depth_div
            return int(new_x)
        
        def scaled_depth(x):
            return int(math.ceil(x*depth_coeff))
        
        self.stem= conv_bn_act(3,scaled_width(32),kernel_size=3,stride=2,bias=False)

        self.blocks=nn.Sequential(
            MBBlock(scaled_width(32) ,scaled_width(16)  ,1,3,1,scaled_depth(1),True,0.25,drop_connect_rate),
            MBBlock(scaled_width(16) ,scaled_width(24)  ,6,3,2,scaled_depth(2),True,0.25,drop_connect_rate),
            MBBlock(scaled_width(24) ,scaled_width(40)  ,6,5,2,scaled_depth(2),True,0.25,drop_connect_rate),
            MBBlock(scaled_width(40) ,scaled_width(80)  ,6,3,2,scaled_depth(3),True,0.25,drop_connect_rate),
            MBBlock(scaled_width(80) ,scaled_width(112) ,6,5,1,scaled_depth(3),True,0.25,drop_connect_rate),
            MBBlock(scaled_width(112),scaled_width(192) ,6,5,2,scaled_depth(4),True,0.25,drop_connect_rate),
            MBBlock(scaled_width(192),scaled_width(320) ,6,3,1,scaled_depth(1),True,0.25,drop_connect_rate)
        )

        self.head=nn.Sequential(
            *conv_bn_act(scaled_width(320),scaled_width(1280),kernel_size=1,bias=False),
            nn.AdaptiveAvgPool2d(1),
            nn.Dropout2d(dropout_rate,True) if dropout_rate >0 else nn.Identity(),
            Flatten(),
            nn.Linear(scaled_width(1280),num_classes)
        )
        self.init_weights()

    def init_weights(self):
        for m in self.modules():
            if isinstance(m, SamePadConv2d):
                nn.init.kaiming_normal_(m.weight,mode ="fan_out")
            elif isinstance(m,nn.Linear):
                init_range=1.0 / math.sqrt(m.weight.shape[1])
                nn.init.uniform_(m.weight,-init_range,init_range)
    
    def forward(self,inputs):
        stem=self.stem(inputs)
        x=self.blocks(stem)
        head=self.head(x)
        return head
