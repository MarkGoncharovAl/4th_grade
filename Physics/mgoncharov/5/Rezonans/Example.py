import MNK as M
import pandas as pd
import numpy as np

graduation = pd.read_excel('data.xls')
print(graduation.to_markdown(index=False))
ln = M.line(np.array(graduation['Data'].tolist()), np.array(graduation['Else'].tolist()), "line")
ln.dump("\n\nLINE")