
from torch.utils.data.dataloader import DataLoader
import pandas as pd
from torchvision.datasets import ImageFolder
import torchvision.transforms as transforms
import os
from utils import DeviceDataLoader

def Load_dataset(root, batch_size, resolution, device):
    normalize = transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                     std=[0.229, 0.224, 0.225])
    
    train_dataset = ImageFolder(
        root + "Train",
        transforms.Compose([
            transforms.Resize([resolution, resolution]),
            transforms.RandomResizedCrop(resolution),
            transforms.RandomHorizontalFlip(),
            
            transforms.ToTensor(),
            normalize,
        ])
    )
    val_dataset = ImageFolder(
        root + "Val",
        transforms.Compose([
            transforms.Resize([resolution, resolution]),
            
            transforms.ToTensor(),
            normalize,
        ])
    )


    train_dl=DataLoader(train_dataset,batch_size=batch_size, shuffle=True,num_workers=2,pin_memory=True)
    val_dl=DataLoader(val_dataset,batch_size=int(batch_size/2), num_workers=2,pin_memory=True)

    train_dl = DeviceDataLoader(train_dl, device)
    val_dl = DeviceDataLoader(val_dl, device)

    return train_dl,val_dl
