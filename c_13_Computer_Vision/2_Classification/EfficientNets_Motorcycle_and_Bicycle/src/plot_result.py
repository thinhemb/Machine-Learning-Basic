import matplotlib.pyplot as plt
import numpy as np
import torch
from sklearn.metrics import confusion_matrix
from sklearn.metrics import ConfusionMatrixDisplay
import os
import torch.nn.functional as F
import pandas as pd
from torch.utils.data.dataloader import DataLoader

from torchvision.datasets import ImageFolder
import torchvision.transforms as transforms

from utils import DeviceDataLoader

from models.EfficientNets import EfficientNet


def plot_accuracies(history):
    val_acc = [x for x in history['val_acc']]
    train_acc = [x for x in history['train_acc']]
    
    plt.plot(train_acc ,label='train_acc',marker = 'o',color='r')
    plt.plot(val_acc,label='val_acc',marker = 'o',color='b')
    plt.grid()
    plt.xlabel('epoch')
    plt.ylabel('acc')
    plt.title(' Accuracy')
    plt.legend()  
    plt.savefig('./output/Accuracies.jpg', dpi=300)
    
def plot_loss(history):
    train_losses = [x for x in history['train_loss']]
    val_losses = [x for x in history['val_loss']]
    plt.plot(train_losses,label='train_loss',marker = 'o')
    plt.plot(val_losses,label='val_loss',marker = 'o')
    plt.xlabel('Epoch')
    plt.ylabel('Loss')
    plt.title(' Loss')
    plt.legend()
    plt.savefig('./output/Loss.jpg', dpi=300)
    
def get_predictions(model, iterator,device):

    model.eval()

    images = []
    labels = []
    probs = []

    with torch.no_grad():

        for (x, y) in iterator:

            x = x.to(device)

            y_pred = model(x)

            y_prob = F.softmax(y_pred, dim = -1)
            top_pred = y_prob.argmax(1, keepdim = True)

            images.append(x.cpu())
            labels.append(y.cpu())
            probs.append(y_prob.cpu())

    images = torch.cat(images, dim = 0)
    labels = torch.cat(labels, dim = 0)
    probs = torch.cat(probs, dim = 0)

    return images, labels, probs

def plot_confusion_matrix(labels, pred_labels, classes):
    
    fig = plt.figure(figsize = (12, 12))
    ax = fig.add_subplot(1, 1, 1)
    cm = confusion_matrix(labels, pred_labels)
    cm = ConfusionMatrixDisplay(cm, display_labels = classes)
    cm.plot(values_format = 'd', cmap = 'Blues', ax = ax)
    fig.delaxes(fig.axes[1]) #delete colorbar
    plt.xticks(rotation = 90)
    plt.xlabel('Predicted Label', fontsize = 20)
    plt.ylabel('True Label', fontsize = 20)
    plt.savefig('./output/test/confusion_matrix.jpg', dpi=300)

def Confusion_matrix(model, path_val,val_dl,device):
    classes = os.listdir(path_val)
    images, labels, probs = get_predictions(model,val_dl ,device)
    pred_labels = torch.argmax(probs, 1)
    plot_confusion_matrix(labels, pred_labels, classes)


if __name__ =='__main__':
    history=pd.read_csv('./output/history_b1.csv')
    plot_accuracies(history)
    plot_loss(history)

    device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model=EfficientNet(1.0, 1.1)
    net=torch.load("./output/model_best_b1.pth",map_location=device)

    model.load_state_dict(net.module.state_dict())
    model.to(device)
    normalize = transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                     std=[0.229, 0.224, 0.225])
    test_dl= ImageFolder(
        './Data/Test',
        transforms.Compose([
            transforms.Resize([224, 224]),
            
            transforms.ToTensor(),
            normalize,
        ])
    )
    test_dl=DataLoader(test_dl,batch_size=int(8), num_workers=2,pin_memory=True)

    
    test_dl = DeviceDataLoader(test_dl, device)
    Confusion_matrix(model, './Data/Test',test_dl,device)
