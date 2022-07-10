import math
from random import random
import torch
import torch.nn as nn 
import torch.nn.functional as F

def conv_bn_act(in_, out_, kernel_size,
                stride=1, groups=1,bias=True,
                eps=1e-3,momentum=0.01):

    return nn.Sequential(
        SamePadConv2d(in_, out_, kernel_size,stride,groups=groups,bias=bias),
        nn.BatchNorm2d(out_,eps,momentum),
        Swish()
        )


class SamePadConv2d(nn.Conv2d):
    def __init__(self,in_channels, out_channels, kernel_size, stride=1, dilation=1, groups=1, bias=True, padding_mode="zeros"):

        super().__init__(in_channels, out_channels, kernel_size, stride,0, dilation, groups, bias, padding_mode)
    
    
    def get_pad_odd(self,in_,weight,stride,dilation):
        effective_filter_size_rows= (weight-1)*dilation +1
        out_rows=(in_+stride-1)//stride
        padding_needed=max(0,(out_rows-1)*stride+effective_filter_size_rows-in_)
        padding_rows=max(0,(out_rows-1)*stride +(weight-1)* dilation+1-in_)
        rows_odd=(padding_rows %2!=0)
        return padding_rows,rows_odd
    
    
    
    def forward(self, x):
        padding_rows,rows_odd=self.get_pad_odd(x.shape[2],self.weight.shape[2],self.stride[0],self.dilation[0])
        padding_cols,cols_odd= self.get_pad_odd(x.shape[3],self.weight.shape[3],self.stride[1],self.dilation[1])
        
        if rows_odd or cols_odd:
            x=F.pad(x,[0, int(cols_odd), 0 , int(rows_odd)])

        return F.conv2d(x, self.weight, self.bias, self.stride,
            padding=(padding_rows//2,padding_cols//2),
            dilation=self.dilation,groups=self.groups)




class Swish(nn.Module):
    def forward(self,x):
        return x*torch.sigmoid(x)




class Flatten(nn.Module):
    def forward(self,x):
        return x.view(x.shape[0],-1)




class SEModule(nn.Module):
    def __init__(self,in_,squeeze_ch):
        super().__init__()
        self.se=nn.Sequential(
            nn.AdaptiveAvgPool2d(1),
            nn.Conv2d(in_,squeeze_ch,kernel_size=1,stride=1,padding=0,bias=True),Swish(),
            nn.Conv2d(squeeze_ch,in_,kernel_size=1,stride=1,padding=0,bias=True)
        )
    def forward(self,x):
        return x * torch.sigmoid(self.se(x))



class DropConnect(nn.Module):
    def __init__(self,ratio):
        super().__init__()
        self.ratio=1.0-ratio
    
    def forward(self,x):
        if not self.training:
            return x
        random_tensor=self.ration
        random_tensor +=torch.rand([x.shape[0],1,1,1],dtype=torch.float,device=x.device)
        random_tensor.requires_grad_(False)
        return x/self.ratio* random_tensor.floor()

