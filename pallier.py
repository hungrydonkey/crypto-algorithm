import phe
import math
import random
import gmpy2
from gmpy2 import *
#m = [4,45,345,345,123,545,234]
pk,sk = phe.generate_paillier_keypair(n_length=1024)



def encrypt(m):
    #print(pk.n)
    #print(pk.g)
    r = random.randint(1,pk.n)
    n2 = pk.nsquare
    rn = pow(r,pk.n,n2)
    #print('rn',rn)
    gm = pow(pk.g,m,n2)#pk.g**m%n2
    #print('gm',gm)
    c = gm * rn %n2
    return c


def decrypt(c):
    #print('sk',sk.p)
    #print(gmpy2.is_prime(sk.p))
    lambda1 = mpz(sk.p-1)*mpz(sk.q-1)
    mu = gmpy2.invert(lambda1,mpz(pk.n))
    #if pk.n == mpz(sk.p)*mpz(sk.q):
        #print('dui le') 

    #tmep = c ^ lam mod n^2 
    temp = gmpy2.powmod(c,lambda1,pk.nsquare)
    #print(temp)
    nn1 = gmpy2.invert(pk.n,2**1024)
    #print(nn1)
    temp = (temp - 1) * nn1 % 2**1024
    #print(temp)
    p = temp * mu % pk.n
    #print(p)
    return p

