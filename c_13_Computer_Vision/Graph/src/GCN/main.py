from re import L
import time
import argparse
import numpy as np

import torch
from zmq import device
from utils import load_data,args,write_keypoint 
from trainer import Train


# Training settings
if __name__=='__main__':
    
    args=args()

    np.random.seed(args.seed) 
    if args.write_data:
        write_keypoint(args)   
    # Load data
    id,labels,path_image,keypoint = load_data(args)
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    t_total=time.time()
    model=Train(args,id,labels,keypoint,device)
    model.Run()


    print("Optimization Finished!")
    print("Total time elapsed: {:.4f}s".format(time.time() - t_total))
    print("===================Start test model===================")
    # Testing
    model.test()