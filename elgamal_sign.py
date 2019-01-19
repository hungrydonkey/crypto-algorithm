import random


z = 1
p = 2016100169
g = random.randint(1,p-1)
x = random.randint(1,p-2)   #private key
k = random.randint(1,p-2)
h = 0                       #public key
m = 23

print('g:',g,'x:',x,'k:',k)

class coef:
    x = 0
    y = 0
    error = 1
        

def exgcd(a,b,c):
    #print(b)
    if b == 0:
        if a == 1:
            c.x = 1
            c.y = 0
            return c
        else:
            c.error = 0
            return c
    else :
        temp = coef()
        temp = exgcd(b,a%b,c)
        if temp.error == 1 :
            #print(temp.x,temp.y,int((a/b)))
            bnm = temp.y
            c.y = temp.x - int((a/b))*temp.y
            c.x =bnm
        
            return c
        else:
            return temp


def dec_sq(a,b,c):
    global z
    if a != 0 :
        dec_sq(int(a/2),int(b),c)
        z = (z * z) % c 
        if int(a)%2 == 1:
            z = z * b % c
        #print(int(a%2))

def keyge():
    global z
    z = 1
    dec_sq(x,g,p)
    h = z
    z =1
    return h

def Sign(k,m):
    global z
    z = 1
    c = coef()
    c.x = 0
    c.y = 0
    dec_sq(k,g,p)
    r = z
    z =1
    #print(r)
    k1 = exgcd(k,p-1,c)
    #k1.x = (k1.x+p-1)%(p-1)
    if k1.error == 0:
        print('k no ni !')
        return 0,0
    else:
        print(k1.x)    
        k1.x = (k1.x+p-1)%(p-1)
        s = (m - x*r) * k1.x %(p-1)
        return r,s

def verification(r,s):
    global z
    z = 1
    dec_sq(s,r,p)
    s1 = z
    z = 1
    print('s1',s1)
    dec_sq(r,h,p)
    s2 = z
    print('s2',s2)
    z = 1
    s1 = s1 * s2 % p
    print(s1)
    dec_sq(m,g,p)
    s2 = z
    z = 1
    print(s2)
    if s2 == s1:
        print('true')
    

h = keyge()
print('public key:',h,'private key:',x)
r,s  = Sign(k,m)
if r == s and s == 0:
    print('no k-1!')
else:
    print(r,s)
    verification(r,s)


