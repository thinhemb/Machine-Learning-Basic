import torch
import numpy as np
import torch.nn as nn
import torch.nn.functional as F
import argparse
from models.EfficientNets import EfficientNet

def to_device(data, device):
    """Move tensor(s) to chosen device"""
    if isinstance(data, (list,tuple)):
        return [to_device(x, device) for x in data]
    return data.to(device, non_blocking=True)

class DeviceDataLoader():
    """Wrap a dataloader to move data to a device"""
    def __init__(self, dl, device):
        self.dl = dl
        self.device = device
        
    def __iter__(self):
        """Yield a batch of data after moving it to device"""
        for b in self.dl: 
            yield to_device(b, self.device)

    def __len__(self):
        """Number of batches"""
        return len(self.dl)

        
def accuracy(outputs, labels):
    _, preds = torch.max(outputs, dim=1)
    return torch.tensor(torch.sum(preds == labels).item() / len(preds))

    
def validation_step(model,batch):
    images, labels = batch 
    out = model(images)                    # Generate predictions
    loss = F.cross_entropy(out, labels)   # Calculate loss
    acc = accuracy(out, labels)           # Calculate accuracy
    return {'val_loss': loss.detach(), 'val_acc': acc}
        
def validation_epoch_end(outputs):
    batch_losses = [x['val_loss'] for x in outputs]
    epoch_loss = torch.stack(batch_losses).mean()   # Combine losses
    batch_accs = [x['val_acc'] for x in outputs]
    epoch_acc = torch.stack(batch_accs).mean()      # Combine accuracies
    return {'val_loss': epoch_loss.item(), 'val_acc': epoch_acc.item()}
    
def epoch_end(result):
    print("  Train_loss: {:.4f}, val_loss: {:.4f}, train_acc: {:.4f}, val_acc: {:.4f}".format(
         result['train_loss'], result['val_loss'],result['train_acc'], result['val_acc']))


def arg_parse():
    # projects description
    desc = "Pytorch EfficientNet"
    parser = argparse.ArgumentParser(description=desc)
    parser.add_argument('--save_dir', type=str,default="./output/",
                        help='Directory name to save the model')

    parser.add_argument('--batch_size', type=int, default=8, help='The size of batch')
    parser.add_argument('--root', type=str, default="./Data/",
                        help="The Directory of data path.")
    parser.add_argument('--gpus', type=str, default="0,1,2,3",
                        help="Select GPU Numbers | 0,1,2,3 | ")

    parser.add_argument('--model', type=str, default='b1',
                        choices=["b1"],
                        help='The type of Efficient net.')

    parser.add_argument('--epochs', type=int, default=40, help='The number of epochs')
    
    parser.add_argument('--test', action="store_true", help='Only Test')

    parser.add_argument('--ema_decay', type=float, default=0.9999,
                        help="Exponential Moving Average Term")

    parser.add_argument('--drop_connect_rate', type=float, default=0.3)

    parser.add_argument('--optim', type=str, default='rmsprop', choices=["rmsprop"])
    parser.add_argument('--lr',    type=float, default=1e-5*5, help="Base learning rate when train batch size is 256.")
    # Adam Optimizer
    parser.add_argument('--beta', nargs="*", type=float, default=(0.5, 0.999))

    parser.add_argument('--momentum', type=float, default=0.9)
    parser.add_argument('--eps',      type=float, default=0.001)
    parser.add_argument('--decay',    type=float, default=1e-5)

    parser.add_argument('--scheduler', type=str, default='exp', choices=["exp", "cosine", "none"],
                        help="Learning rate scheduler type")

    return parser.parse_args()


def get_model(arg, classes=1000):
    if arg.model == "b0":
        return EfficientNet(1.0, 1.0, num_classes=classes,dropout_rate=0.2)
    elif arg.model == "b1":
        return EfficientNet(1.0, 1.1, num_classes=classes,dropout_rate=0.2)
    elif arg.model == "b2":
        return EfficientNet(1.1, 1.2, num_classes=classes,dropout_rate=0.3)
    elif arg.model == "b3":
        return EfficientNet(1.2, 1.4, num_classes=classes,dropout_rate=0.3)
    elif arg.model == "b4":
        return EfficientNet(1.4, 1.8, num_classes=classes,dropout_rate=0.4)
    elif arg.model == "b5":
        return EfficientNet(1.6, 2.2, num_classes=classes,dropout_rate=0.4)
    elif arg.model == "b6":
        return EfficientNet(1.8, 2.6, num_classes=classes,dropout_rate=0.5)
    elif arg.model == "b7":
        return EfficientNet(2.0, 3.1, num_classes=classes,dropout_rate=0.5)