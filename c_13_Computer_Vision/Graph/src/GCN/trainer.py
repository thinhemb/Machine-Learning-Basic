import time
import torch
import numpy as np
import torch.nn.functional as F
import torch.optim as optim
from sklearn.model_selection import train_test_split
from tqdm import tqdm 

from utils import accuracy

from models import GCN


class Train():
    def __init__(self,args,labels,keypoint,device):
        self.args=args
        self.train_dl,self.train_id,self.val_dl,self.val_id=train_test_split(keypoint,labels,test_size=0.2, random_state=4)
        self.device=device
        self.model=GCN(nfeat=self.train.shape[1],
                            nhid=args.hidden,
                            nclass=labels.max().item() + 1,
                            dropout=args.dropout)
        if self.args.pretrain == True:
            self.model=torch.load_state_dict(torch.load(self.args.model))
        self.optimizer = optim.Adam(self.model.parameters(),
                                    lr=args.lr, 
                                    weight_decay=args.weight_decay)
    def Run(self):
        print("===================Start train model===================")
        
        torch.manual_seed(self.args.seed)
        if self.args.cuda:
            torch.cuda.manual_seed(self.args.seed)
        self.model.to(self.device)
        best=0
        train_iterator=tqdm(range(self.args.epochs),desc="Epoch")
        for epoch in train_iterator:
            self.model.train()

            train_losses = []
            train_acc=[]
            for id,keypoint in self.train_dl:
                
                output=self.model(keypoint)
                loss_train = F.nll_loss(output, self.train_id[id])
                acc_train = accuracy(output,  self.train_id[id])
                train_losses.append(loss_train)
                train_acc.append(acc_train)
                loss_train.backward()
                self.optimizer.step()
                self.optimizer.zero_grad()
            with torch.no_grad():
                self.model.eval()
                val_losses,val_acc=self.evaluate()
            if best<val_acc:
                torch.save(self.model.state_dict(),"./pretrain/GCN_best.pt")
            torch.save(self.model.state_dict(),"./pretrain/GCN_last.pt")

            train_iterator.set_description(f"Training loss = {loss_train.item():.4f}, "
                                        f"acc_train ={acc_train.item():.4f}, "
                                        f"val loss = {val_losses:.4f}, val accuracy = {val_acc:.2f}")

            # print('Epoch: {:04d}'.format(epoch+1),
            #     'loss_train: {:.4f}'.format(loss_train.item()),
            #     'acc_train: {:.4f}'.format(acc_train.item()),
            #     'loss_val: {:.4f}'.format(loss_val.item()),
            #     'acc_val: {:.4f}'.format(acc_val.item()),
            #     'time: {:.4f}s'.format(time.time() - t_start))
    
    
    def test(self):
        self.model.eval()
        loss_test,acc_test=self.evaluate()
        print("Test set results:",
            "loss= {:.4f}".format(loss_test.item()),
            "accuracy= {:.4f}".format(acc_test.item()))

    def evaluate(self):
        val_losses = []
        val_acc=[]
        for id,keypoint in self.val_dl:
            output = self.model(keypoint)

            loss_val = F.nll_loss(output, self.val_id[id])
            acc_val = accuracy(output ,self.val_id[id])
            val_losses.append(loss_val)
            val_acc.append(acc_val)
        return val_losses,val_acc