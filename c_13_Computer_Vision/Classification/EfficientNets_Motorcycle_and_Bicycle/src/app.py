import uvicorn
import cv2,numpy as np
from fastapi import FastAPI, File, UploadFile
from starlette.responses import RedirectResponse
from models.EfficientNets import EfficientNet
import aiofiles
import torch
from torchvision.datasets import ImageFolder
import torchvision.transforms as transforms

app = FastAPI(title=' FastAPI Starter Pack')
device=torch.device("cuda" if torch.cuda.is_available() else "cpu")
path_train='./Data/Train'
transformations = transforms.Compose([transforms.Resize((224, 224)), transforms.ToTensor()])
transform = transforms.ToTensor()
dataset = ImageFolder(path_train, transform = transformations)


@app.get("/", include_in_schema=False)
async def index():
    return RedirectResponse(url="/docs")

def to_device(data, device):
    """Move tensor(s) to chosen device"""
    if isinstance(data, (list,tuple)):
        return [to_device(x, device) for x in data]
    return data.to(device, non_blocking=True)

def predict(img,model):
    image= transform(img)
    xb = to_device(image.unsqueeze(0), device)

    # Get predictions from model    
    yb = model(xb)
    # Pick index with highest probability
    prob, preds  = torch.max(yb, dim=1)
    # Retrieve the class label
    return dataset.classes[preds[0].item()]
    

@app.post("/predict")
async def predict_api(file: UploadFile = File(...)):
    extension = file.filename.split(".")[-1] in ("jpg", "jpeg", "png")
    if not extension:
        return {"Status":"Image must be jpg or png format!"}

    path='./uploaded_images/'+file.filename
    
    async with aiofiles.open(path, 'wb') as out_file:
        while content := await file.read(1024):  # async read file chunk
            await out_file.write(content)  # async write file chunk
    image =  cv2.imread(path)
    
    model=EfficientNet(1.0, 1.1)
    net=torch.load("./output/model_best_b1.pth",map_location=device)

    model.load_state_dict(net.module.state_dict())
    model.to(device)
    ret = predict(image,model)
    result= ({"Name":ret })

    return result




if __name__ == "__main__":
    uvicorn.run(app, debug=True)