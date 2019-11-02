import numpy as np 
import sys 
import copy
def getbase():
    base = []
    argv = sys.argv
    if len(argv) < 2:
        print('need a file name!')
        sys.exit()
    with open(argv[1],'r') as fp:
        data = fp.readlines()
        fp.close()
    for i in data:
        i = i.replace('[',' ').replace(']',' ').replace('\n',' ').split(' ')
        i = [i[j] for j in range(len(i)) if i[j] != '']
        if i:
            i = list(map(int,i))
            base.append(i)
    return base

def sqrmo(v):
    mo2 = 0
    for i in v:
        mo2 += i**2
    return mo2

def toint(i):
    return int(i+0.5) if i >= 0 else int(i-0.5) 

def vectop(a,op,b):
    if op == '*':
        return np.dot(np.array(a,dtype = np.float64),np.array(b,dtype = np.float64))
    elif op == '-':
        rel = []
        for i in range(len(a)):
            rel.append(a[i] - b[i]) 
        return rel
    else:
        rel = [b[i]*a for i in range(len(b))]
        return rel

def Reduce(k,l,mu,lbase):
    if abs(mu[k][l]) <= 0.5:
        return
    m = toint(mu[k][l])
    lbase[k] = vectop(lbase[k],'-',vectop(m,'mul',lbase[l]))
    mu[k][l] = mu[k][l] - m
    for i in range(l):
        mu[k][i] = mu[k][i] - m * mu[l][i]

def swapvect(k,mu,mo,lbase):
    lbase[k-1],lbase[k] = lbase[k],lbase[k-1]
    
    μ = mu[k][k-1] 
    B = mo[k] + mo[k-1] * μ**2
    mu[k][k-1] = μ * mo[k-1] / B
    mo[k] = mo[k-1] * mo[k]  / B
    mo[k-1] = B
    for j in range(k-1):
        mu[k-1][j],mu[k][j] = mu[k][j],mu[k-1][j]
    for i in range(k+1,len(lbase)):
        m = mu[i][k]
        mu[i][k]   = mu[i][k-1] - μ * m
        mu[i][k-1] = m + mu[k][k-1] * mu[i][k]

def lll_reduce(base,α):
    #α = 0.75
    n = len(base)
    vbase = [0] * n
    lbase = copy.deepcopy(base)
    mo = [0] * n
    mu = np.zeros((n,n))
    for i in range(n):
        vbase[i] = lbase[i]
        for j in range(i):
            mu[i][j] = vectop(lbase[i],'*',vbase[j]) / mo[j]
            vbase[i] = vectop(vbase[i],'-',vectop(mu[i][j],'mul',vbase[j]))
        mo[i] = sqrmo(vbase[i])
    k = 1
    while k < n :
        Reduce(k,k-1,mu,lbase)
        if mo[k] >= (α - mu[k][k-1]**2) * mo[k-1] :
            for l in range(k-2,-1,-1):
                Reduce(k,l,mu,lbase)
            k = k + 1
        else :
            swapvect(k,mu,mo,lbase)
            k = k - 1 if k > 1 else k
    return lbase
        
def hadema(base):
    s = 1
    for i in range(len(base)):
        s *= sqrmo(base[i])**(1/2)
    det = abs(round(np.linalg.det(np.array(base,dtype = np.float64))))
    return (det/s)**(1/len(base))
'''
base = getbase()
lbase = lll_reduce(base,0.75)
if len(lbase) < 10:
    print(np.array(lbase))
else:
    print(lbase)
'''
