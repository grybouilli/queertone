import torch

class LinearPred(torch.nn.Module):
    def __init__(self, input_dim, output_dim):
        super(LinearPred, self).__init__()
        self.layer = torch.nn.Linear(input_dim, output_dim)
       
    def forward(self, x):
        x = self.layer(x)
        return x