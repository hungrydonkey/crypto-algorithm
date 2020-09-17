from sympy import *
from sympy.abc import x
import random 
import numpy
import math
N = 150
d = 30
p = 3
q = 43
R = x**N - 1
#print(R)

#example
#fx = Poly(x**6-x**4+x**3+x**2-1,x)
#gx = Poly(x**6+x**4-x**2-x,x)
#m  = Poly(-x**5+x**3+x**2-x+1,x)
#rx = Poly(x**6-x**5+x-1)

#fx_1q = invert(fx, R, domain=GF(q))
#fx_1p = invert(fx, R, domain=GF(p))
#print("fx_1q is ", fx_1q)
#print("fx_1p is ", fx_1p)
#s,hx    = div(fx_1q * gx, R, domain=GF(q))
#print("hx is ", hx)
#s,ex = div(p * rx * hx + m, R,domain=GF(q))
#print("e is ",ex)
#s,temp = div(fx * ex, R, domain=GF(q))
#print("fx * ex is ",temp)
#ax= temp

#s,m_d = div(fx_1p * ax, R, domain=GF(p))
#print(m_d)
#example

def genpq():
    p = 0
    q = 0
    while 1:
        p = randprime(50,100)
        while 1:
            q = random.randint(int((6*d + 1) * p), 2**16)
            if q > (6*d+1)*p and gcd(p,q) == 1 and gcd(N,q) == 1:
                break
        break
    return p,q 

'''
def invert_poly(f_poly, R_poly, p):
    inv_poly = None
    if isprime(p):
        inv_poly = invert(f_poly, R_poly, domain=GF(p))
    elif (p & (p-1)) == 0:
        inv_poly = invert(f_poly, R_poly, domain=GF(2))
        e = int(math.log(p, 2))
        for i in range(1, e):
            s,temp = div((2 * inv_poly - f_poly * inv_poly ** 2),R_poly, domain=GF(p))
            inv_poly = temp
    return inv_poly
'''


def randompoly(l, d1, d2):
    #d1  = 1 , d2 = -1
    a = numpy.concatenate((numpy.zeros(l - d1 - d2, int), numpy.ones(d1, int), -numpy.ones(d2, int)))
    a = numpy.random.permutation(a)
    #print(a) 
    p = Poly(a, x)
    #print(p, p.eval(1))
    return p

def genrandkey():
    gx    = randompoly(N, d, d)
    fx    = None
    fx_1q = None
    fx_1p = None
    for i in range(100):
        try:
            fx = randompoly(N, d+1, d)
            fx_1q = invert(fx, R, domain=GF(q))
            fx_1p = invert(fx, R, domain=GF(p))
        except NotInvertible as e:
            fx = None
        if fx != None:
            break
    if fx is None:
         raise Exception("Couldn't generate invertible f")
    return fx,gx,fx_1p,fx_1q

def genpk(fx_1q,gx):
    s, hx = div(fx_1q * gx, R, domain=GF(q))
    return hx

def encrypt(hx, m):
    rx = randompoly(N, d, d)
    ex = p * hx * rx + m 
    s, ex = div(ex, R, domain=GF(q))
    return ex

def decrypt(ex, fx, fx_1p):
    s, ax = div(fx * ex, R, domain=GF(q)) 
    ax    = ax.set_domain(ZZ)
    fx_1p = fx_1p.set_domain(ZZ)
    s, bx = div(fx_1p * ax, R, domain=GF(p))
    return bx


m = Poly(list(map(int, list("1202011217"))),x)
print(m)
#randompoly(10, 3, 2)
p,q = genpq()
print("p,q is ",p,q)
#print(p,q)
#fx = randompoly(N,d+1,d)
#gx = randompoly(N,d,d)
#print(fx,"\n",gx)
fx,gx,fx_1p,fx_1q = genrandkey()
print("private keys is ",fx,fx_1q)
hx = genpk(fx_1q, gx)
print("public key hx is ",hx)
ex = encrypt(hx, m)
print("cipher is ",ex)

bx = decrypt(ex, fx, fx_1p)
print("decode is ",bx,bx.all_coeffs())