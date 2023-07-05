# This file was made following this tutorial : https://pytorch.org/tutorials/beginner/introyt/trainingyt.html

import torch
from torch import nn
from torch import optim
import numpy as np
from sklearn.model_selection import train_test_split
import matplotlib.pyplot as plt
from torch.utils.data import Dataset, DataLoader


import nn_tilde

# The function to guess is f(x) = a*x+b
a = 5
b = 7

# Data preparation
NBPOINTS = 100

# generate data
datax = np.array([(x / 100 + 1) for x in range(0, NBPOINTS)]).reshape(-1,1)
print(datax.shape)
datay = datax * a + b  # This applies on all the elements of the vector

# We add some noise to the perfect line
datay += np.random.normal(0, 0.05, NBPOINTS).reshape(NBPOINTS,1)

X_train, X_test, y_train, y_test = train_test_split(datax, datay, test_size=.33, random_state=26)

# Convert data to torch tensors
class Data(Dataset):
    def __init__(self, X, y):
        self.X = torch.from_numpy(X.astype(np.float32))
        self.y = torch.from_numpy(y.astype(np.float32))
        self.len = self.X.shape[0]
       
    def __getitem__(self, index):
        return self.X[index], self.y[index]
   
    def __len__(self):
        return self.len
    
batch_size = 64

# Instantiate training and test data
train_data = Data(X_train, y_train)
train_dataloader = DataLoader(dataset=train_data, batch_size=batch_size, shuffle=True)

test_data = Data(X_test, y_test)
test_dataloader = DataLoader(dataset=test_data, batch_size=batch_size, shuffle=True)

# Check it's working
for batch, (X, y) in enumerate(train_dataloader):
    print(f"Batch: {batch+1}")
    print(f"X shape: {X.shape}")
    print(f"y shape: {y.shape}")
    break

class LinearPred(nn_tilde.Module):
    def __init__(self, input_dim, output_dim):
        super(LinearPred, self).__init__()
        self.layer = nn.Linear(input_dim, output_dim)
        self.register_method(
            'forward',
            in_channels=1,
            in_ratio=1,
            out_channels=1,
            out_ratio=1,
            test_buffer_size = 1,
        )
    @torch.jit.export
    def forward(self, x):
        x = self.layer(x)
        return x
    
model = LinearPred(1,1)
print(model)


loss_fn = nn.MSELoss()

learning_rate = 0.3
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate)

num_epochs = 250
loss_values = []

print('Weight and bias (before training)')
print(model.layer.weight)
print(model.layer.bias)

for epoch in range(num_epochs):
    for X, y in train_dataloader:
        # zero the parameter gradients
        optimizer.zero_grad()
       
        # forward + backward + optimize
        pred = model(X)
        loss = loss_fn(pred, y)
        loss_values.append(loss.item())
        loss.backward()
        optimizer.step()

print("Training Complete")


print('Weight and bias (after training)')
print(a, "approximated by ", round(float(model.layer.weight), 4))
print(b, "approximated by ", round(float(model.layer.bias), 4))

model.export_to_ts('lin-pred.ts')
torch.save(model, "runs/best-lin-pred.pt")
torch.save(model.state_dict(), "runs/best-lin-pred-dict.pt")
print("Model saved")
# Plot the loss values
# plt.plot(np.arange(len(loss_values)), np.array(loss_values))
# plt.title("Step-wise Loss")
# plt.xlabel("Epochs")
# plt.ylabel("Loss")
# plt.show()