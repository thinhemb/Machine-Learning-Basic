import time
import torch
import numpy as np
import torch.nn.functional as F
import torch.optim as optim
from utils import accuracy

from models import GCN


class Train():
    def __init__(self,args):
        self.args=args
        self.data=""
        self.model=GCN(nfeat=features.shape[1],
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
            self.model.cuda()
        best=0
        for epoch in range(0,self.args.epochs):
            t_start=time.time()
            self.model.train()
            self.optimizer.zero_grad()
            output=self.model(self.data)
            loss_train = F.nll_loss(output[idx_train], labels[idx_train])
            acc_train = accuracy(output[idx_train], labels[idx_train])
            loss_train.backward()
            self.optimizer.step()
            if not self.args.fastmode:
        # Evaluate validation set performance separately,
        # deactivates dropout during validation run.
                self.model.eval()
                output = self.model(features, adj)

            loss_val = F.nll_loss(output[idx_val], labels[idx_val])
            acc_val = accuracy(output[idx_val], labels[idx_val])
            if best<acc_val:
                torch.save(self.model.state_dict(),"./pretrain/GCN_best.pt")
            elif epoch+1==self.arges.epochs:
                torch.save(self.model.state_dict(),"./pretrain/GCN_last.pt")
                
            print('Epoch: {:04d}'.format(epoch+1),
                'loss_train: {:.4f}'.format(loss_train.item()),
                'acc_train: {:.4f}'.format(acc_train.item()),
                'loss_val: {:.4f}'.format(loss_val.item()),
                'acc_val: {:.4f}'.format(acc_val.item()),
                'time: {:.4f}s'.format(time.time() - t_start))
        
    def test(self):
        self.model.eval()
        output = self.model(features, adj)
        loss_test = F.nll_loss(output[idx_test], labels[idx_test])
        acc_test = accuracy(output[idx_test], labels[idx_test])
        print("Test set results:",
            "loss= {:.4f}".format(loss_test.item()),
            "accuracy= {:.4f}".format(acc_test.item()))