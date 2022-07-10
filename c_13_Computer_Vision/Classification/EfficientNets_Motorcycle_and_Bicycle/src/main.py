import os
import argparse
import pandas as pd

import torch
import torch.nn as nn
from torch.optim.lr_scheduler import StepLR
from torch.optim.lr_scheduler import CosineAnnealingLR
import torch.nn.functional as F

from models.EfficientNets import EfficientNet

# from plot_result import *
from loader import Load_dataset
from utils import accuracy, arg_parse, epoch_end, get_model,to_device,validation_step, validation_epoch_end

def get_scheduler(optim, scheduler_type, step_size, t_max):
    if scheduler_type == "exp":
        return StepLR(optim, step_size, 0.97)
    elif scheduler_type == "cosine":
        return CosineAnnealingLR(optim, t_max)
    else:
        return None

@torch.no_grad()
def evaluate(model, val_dl):
    model.eval()
    
    outputs = [validation_step(model,batch) for batch in val_dl]
    return validation_epoch_end(outputs)

def fit(arg, optim, model, train_dl, val_dl,device,scheduler):
    history = []
    best=0
    print("\nStart Train len :", len(train_dl))
    for epoch in range(1,arg.epochs+1):
        # Training Phase 
        print('Epoch {}/{}:'.format(epoch, arg.epochs))
        model.train()
        train_losses = []
        train_acc=[]
        
        for images, labels in train_dl:
            images = to_device(images, device)
            
            out =  model(images)             # Generate predictions
            loss = F.cross_entropy(out, labels)
            acc_train = accuracy(out, labels)           # Calculate accuracy
            train_losses.append(loss)
            train_acc.append(acc_train)
            loss.backward()
            optim.step()
            optim.zero_grad()
            if scheduler:
                scheduler.step()
        # Validation phase
        result = evaluate(model, val_dl)
        result['train_loss'] = torch.stack(train_losses).mean().item()
        result['train_acc'] = torch.stack(train_acc).mean().item()
        epoch_end(result)
        history.append(result)
        if result['val_acc']> best:
            torch.save(model, arg.save_dir+'model_best_{}.pth'.format(arg.model))
        best=max(result['val_acc'],best)
    torch.save(model, arg.save_dir+'model_last_{}.pth'.format(arg.model))
    
    return history

if __name__ == '__main__':
    arg= arg_parse()
    
    if os.path.exists(arg.save_dir) is False:
        os.mkdir(arg.save_dir)
    device =torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')
    
    classes = os.listdir(arg.root+'Train')
    train_dl, val_dl = Load_dataset(arg.root, arg.batch_size, 224, device)

    net=get_model(arg)
    net = nn.DataParallel(net).to(device)
    loss = nn.CrossEntropyLoss()

    scaled_lr = arg.lr * arg.batch_size / 256
    optim = {
        # "adam" : lambda : torch.optim.Adam(net.parameters(), lr=arg.lr, betas=arg.beta, weight_decay=arg.decay),
        "rmsprop" : lambda : torch.optim.RMSprop(net.parameters(), 
        lr=scaled_lr, momentum=arg.momentum, eps=arg.eps, weight_decay=arg.decay)
    }[arg.optim]()

    scheduler = get_scheduler(optim, arg.scheduler, int(2.4 * len(train_dl)), arg.epochs * len(train_dl))

    history = fit(arg,optim, net, train_dl, val_dl,device,scheduler)
    H = pd.DataFrame(history)

    H.to_csv(arg.save_dir+"history_{}.csv".format(arg.model))
    evaluate(net, val_dl)

    # plot_accuracies(history)
    # plot_loss(history)
    # Confusion_matrix(net, arg.root+'Val',val_dl,device)