import random



p = 2016100169
g = random.randint(1,p-1)
d = random.randint(1,p-2) 
k = random.randint(1,p-2)
y = 0
m = 15

print('g:',g,'d:',d,'k:',k)

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
    if a != 0 :
        z = dec_sq(int(a/2),int(b),c)
        z = (z * z) % c
   
        if int(a)%2 == 1:
            z = z * b % c
            return z
        else:
            return z
    else:
        return 1

def keyge():
    return dec_sq(d,g,p)
     
def encrypt():
    c1 = dec_sq(k,g,p)
    print(m)
    c2 = (m*dec_sq(k,y,p))%p
    print(c1,c2)
    return c1,c2

def decrypt(c1,c2):
    kr = dec_sq(d,c1,p)
    print('kr',kr)
    c = coef()
    kr1= exgcd(kr,p,c)
    if kr1.error == 0:
        print('k no ni !')
        return 0,0
    else:
 
        kr1.x = (kr1.x+p)%p
        print(kr1.x)  
        m = c2*kr1.x % p
        print(m)
        return m


y = keyge()
c1,c2 = encrypt()
m1 = decrypt(c1,c2)
print(m1)

