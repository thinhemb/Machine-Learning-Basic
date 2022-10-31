import numpy as np
import scipy.sparse as sp
import torch
import cv2  
from pose_hands import Pose_hands
import pandas as pd
import os
import argparse
import csv

def load_data(args):
    
    if os.path.exists(args.path_data):
        data=pd.read_csv(args.path_data)
        label,path_image, keypoint=data["label"],data["path_image"],data["keypoint"]
        return label,path_image, keypoint
    else:
        print("file does not exist ")
    
        

def write_keypoint(args):
    print('Loading {} dataset...')
    list_lables=os.listdir(args.path_dataset)
    with open('./data.json', 'a',newline="") as outfile:
        writer = csv.writer(outfile)
        writer.writerow(["id","label","path_image", "keypoint"])
        for label in list_lables:
            path_label=args.path+label
            for image in os.listdir(path_label):
                
                try:
                    path_image=path_label+'/'+image
                    image=cv2.imread(path_image)
                    process=Pose_hands(image)
                    image,keypoint=process.keypoint_hands()
                    writer.writerow([id,label,path_image, *keypoint])
                except:
                    pass            

def normalize(mx):
    """Row-normalize sparse matrix"""
    rowsum = np.array(mx.sum(1))
    r_inv = np.power(rowsum, -1).flatten()
    r_inv[np.isinf(r_inv)] = 0.
    r_mat_inv = sp.diags(r_inv)
    mx = r_mat_inv.dot(mx)
    return mx


def accuracy(output, labels):
    preds = output.max(1)[1].type_as(labels)
    correct = preds.eq(labels).double()
    correct = correct.sum()
    return correct / len(labels)



def args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--no-cuda', action='store_true', default=True,
                        help='Disables CUDA training.')
    parser.add_argument('--fastmode', action='store_true', default=False,
                        help='Validate during training pass.')
    parser.add_argument('--seed', type=int, default=42, help='Random seed.')
    parser.add_argument('--epochs', type=int, default=200,
                        help='Number of epochs to train.')
    parser.add_argument('--lr', type=float, default=0.01,
                        help='Initial learning rate.')
    parser.add_argument('--weight_decay', type=float, default=5e-4,
                        help='Weight decay (L2 loss on parameters).')
    parser.add_argument('--hidden', type=int, default=16,
                        help='Number of hidden units.')
    parser.add_argument('--dropout', type=float, default=0.5,
                        help='Dropout rate (1 - keep probability).')
    parser.add_argument('--path_data', type=str, default="./data.csv",
                        help='path dataset.')
    parser.add_argument('--path_dataset', type=str, default="/home/thinh_do/Study/Machine-Learning-Basic/c_13_Computer_Vision/Graph/data/",
                        help='path dataset image.')
    parser.add_argument('--model', type=str, default="best.pt",
                        help='path model.')
    parser.add_argument('--pretrain', action='store_true', default=False,
                        help='check model pretrain.')
    parser.add_argument('--write_data',  action='store_true', default=False,
                        help='check write keypoint to data.json')
    args = parser.parse_args()
    args.cuda = not args.no_cuda and torch.cuda.is_available()
    return args