from models.EfficientNets import EfficientNet
import aiofiles
import torch
from torchvision.datasets import ImageFolder
import torchvision.transforms as transforms
from utils import DeviceDataLoader
from torch.utils.data.dataloader import DataLoader
from plot_result import Confusion_matrix


def main():
    device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model=EfficientNet(1.0, 1.1)
    net=torch.load("./output/model_best_b1.pth",map_location=device)

    model.load_state_dict(net.module.state_dict())
    model.to(device)



    normalize = transforms.Normalize(mean=[0.485, 0.456, 0.406],
                                     std=[0.229, 0.224, 0.225])
    test_dl= ImageFolder(
        './Data/test',
        transforms.Compose([
            transforms.Resize([224, 224]),
            transforms.ToTensor(),
            normalize,
        ])
    )
    test_dl=DataLoader(test_dl,batch_size=int(4), num_workers=2,pin_memory=True)
    test_dl = DeviceDataLoader(test_dl, device)
    Confusion_matrix(model, './Data/test',test_dl,device)




if __name__ =='__main__':
    main()

    
    