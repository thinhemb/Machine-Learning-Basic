import torch.nn as nn
import torch.nn.functional as F
import torch
from layers import GCNConv


class GCN(nn.Module):
    def __init__(self, input_size, hidden_size, output_size, num_hidden_layers=0, dropout=0.1, residual=True):
        super(GCN, self).__init__()

        self.dropout = dropout
        self.residual = residual

        self.input_conv = GCNConv(input_size, hidden_size)
        self.hidden_convs = nn.ModuleList([GCNConv(hidden_size, hidden_size) for _ in range(num_hidden_layers)])
        self.output_conv = GCNConv(hidden_size, output_size)

    def forward(self, x: torch.Tensor, adjacency_hat: torch.sparse_coo_tensor, labels: torch.Tensor = None):
        x = F.dropout(x, p=self.dropout, training=self.training)
        x = F.relu(self.input_conv(x, adjacency_hat))
        for conv in self.hidden_convs:
            if self.residual:
                x = F.relu(conv(x, adjacency_hat)) + x
            else:
                x = F.relu(conv(x, adjacency_hat))
        x = F.dropout(x, p=self.dropout, training=self.training)
        x = self.output_conv(x, adjacency_hat)

        if labels is None:
            return x

        loss = nn.CrossEntropyLoss()(x, labels)
        return x, loss