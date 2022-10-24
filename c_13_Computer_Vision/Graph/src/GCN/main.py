import time
import argparse
import numpy as np

import torch
from utils import load_data,args,write_keypoint 
from trainer import Train

# Training settings
if __name__=='__main__':
    
    args=args()

    np.random.seed(args.seed)
    if args.write_data== True:
        write_keypoint()
    
    # Load data
    adj, features, labels, idx_train, idx_val, idx_test = load_data(args.path_data)

# Model and optimizer

    if args.cuda:
        features = features.cuda()
        adj = adj.cuda()
        labels = labels.cuda()
        idx_train = idx_train.cuda()
        idx_val = idx_val.cuda()
        idx_test = idx_test.cuda()

    t_total=time.time()
    model=Train(args)
    model.Run()


    print("Optimization Finished!")
    print("Total time elapsed: {:.4f}s".format(time.time() - t_total))
    print("===================Start test model===================")
    # Testing
    model.test()