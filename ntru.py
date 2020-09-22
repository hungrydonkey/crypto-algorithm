from sympy import *
from sympy.abc import x
from numpy import random 
import numpy 
import math
N = 79#107,167,503
d = 6
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
        p = randprime(2,40)
        while 1:
            q = random.randint(int((6*d + 1) * p), 2**12)
            if q > (6*d+1)*p and gcd(p,q) == 1 and gcd(N,q) == 1:
                break
        break
    return p,q 


def invert_poly(f_poly, R_poly, p):
    inv_poly = None
    if isprime(p):
        inv_poly = invert(f_poly, R_poly, domain=GF(p))
    elif (p & (p-1)) == 0:
        inv_poly = invert(f_poly, R_poly, domain=GF(2))
        e = int(math.log(p, 2))
        for i in range(1, e):
            s,temp = div((2 * inv_poly - f_poly * inv_poly ** 2),R_poly, domain=GF(p))
            inv_poly = temp.set_domain(ZZ)
    return inv_poly



def randompoly(l, d1, d2):
    #d1  = 1 , d2 = -1
    a = numpy.concatenate((numpy.zeros(l - d1 - d2, int), numpy.ones(d1, int), -numpy.ones(d2, int)))
    a = numpy.random.permutation(a)
    p = Poly(a, x)
    return p

def genrandkey(p,q):
    gx    = randompoly(N, d, d)
    fx    = None
    fx_1q = None
    fx_1p = None
    for i in range(5000):
        try:
            fx = randompoly(N, d+1, d)
            #fx_1q = invert(fx, R, domain=GF(q))
            #fx_1p = invert(fx, R, domain=GF(p))
            fx_1q = invert_poly(fx, R, q)
            fx_1p = invert_poly(fx, R, p)
        except NotInvertible as e:
            fx = None
        if fx != None:
            break
    if fx is None:
         raise Exception("Couldn't generate invertible f")
    return fx,gx,fx_1p,fx_1q

def genpk(fx_1q, gx, q):
    s, hx = div(fx_1q * gx, R, domain=GF(q))
    return hx

def encrypt(hx, m, p, q):
    rx = randompoly(N, d, d)
    print("rx is ",rx)
    print(rx.all_coeffs())
    ex = p * hx * rx + m 
    s, ex = div(ex, R, domain=GF(q))
    return ex
    #temp = ex - m
    #print("c - m (1) is ", temp.eval(1))
    #i = mod_inverse(p, q)
    #z = i * temp 
    #z = z.set_modulus(q)
    #print("p-1 * (c - m) is ", z, z.eval(1))

def decrypt(ex, fx, fx_1p, p, q):
    s, ax = div(fx * ex, R, domain=GF(q)) 
    ax    = ax.set_domain(ZZ)
    fx_1p = fx_1p.set_domain(ZZ)
    s, bx = div(fx_1p * ax, R, domain=GF(p))
    return bx

def dbdecrypt(cp, m, fx, fx_1p, p, q):
    s, ax = div(fx * cp, R, domain=GF(q))
    ax    = ax.set_domain(ZZ)
    fx_1p = fx_1p.set_domain(ZZ)
    s, bx = div(fx_1p * ax, R, domain=GF(p))
    print("decode cp is ", bx.all_coeffs())
    if m.all_coeffs() == bx.all_coeffs():
        return True
    else:
        return False

def detect1(p, q, c, m):
    inverse_p = mod_inverse(p, q)
    z   = ( c - m ) * inverse_p
    z   = z.set_modulus(q)
    #print("z is ", z)

    z_1 = z.eval(1)
    print("z(1) is ", z_1)
    return z_1

def detect2(p, q, hx, c, m):
    print(c)
    print(m)
    φ1  = Poly(x - 1)
    φ2  = div(R, φ1)[0]
    inverse_p = mod_inverse(p, q)
    w   =  ( c - m ) * inverse_p
    w   =  w.set_modulus(q)
    print(w, w.eval(1))
    s, w   =  div(w, φ2, domain=GF(q))
    print(w)
    try:
        inverse_hx_inφ2 = invert_poly(hx, φ2, q)
    except NotInvertible:
        raise Exception("Couldn't generate invertible hx in φ2")
    print("inverse of hx is ", inverse_hx_inφ2)
    print(div(inverse_hx_inφ2 * hx, φ2, domain=GF(q))[1])

    w_φ1    = 0
    s, w_φ2 = div(w * inverse_hx_inφ2, φ2, domain=GF(q))
    print(w_φ2)
    
    inverse_φ1_inφ2  =  invert(φ1, φ2, domain=GF(q))
    print("inverse_φ1 is ", inverse_φ1_inφ2)
    s, w    = div(w_φ2 * inverse_φ1_inφ2  , φ2, domain=GF(q))
    print(w)
    s, w    = div(φ1 *  w, R, domain=GF(q))
    print(w, w.all_coeffs())
    return None

def ntru():
    #hx = genpk(fx_1q, gx, q)
    #print("public key hx is ",hx.all_coeffs())
    #print("hx(1) is ", hx.eval(1))

    ex = encrypt(hx, m, p, q)
    #print("cipher is ",ex.all_coeffs())
    #print("ex(1) is ", ex.eval(1))

    bx = decrypt(ex, fx, fx_1p, p, q)
    print("decode is ",bx,bx.all_coeffs())
    if bx.all_coeffs() == m.all_coeffs():
        print("Decrypt True")
    return ex


def bdntru(ex):
    ρ     = 2 
    k     = None
    print(ρ < q, gcd(ρ, p) == 1)

    coeff_e =  ex.all_coeffs()
    n_e     =  len(coeff_e)
    print("ex's coeffs is", coeff_e)
    print("len(ex) is", n_e)

    k  = random.randint(0,3,n_e)
    k  = k[::-1]
    #print("k is", k)
    C  = []
    cp = []
    for i in range(n_e):
        cpi = coeff_e[i] + p * k[i]
        ci = cpi % ρ
        C.append(ci)
        cp.append(cpi)
    #print("C is ",C)
    cp = Poly(cp,x)
    #print("cp is",cp)

    coeff_cp = cp.all_coeffs()
    dC = []
    for i in range(n_e):
        dC.append(coeff_cp[i] % ρ)
    #print("dC is ",dC)
    print("get M is", C == dC)
    print("Back door is right? ", dbdecrypt(cp, m, fx, fx_1p, p, q))
    return cp

def initial():
    p     = 3#0
    q     = 128#0
    fx    = None
    gx    = None
    fx_1q = None
    fx_1p = None
    #while 1:
    #    p,q = genpq()
    #    try:
    #        fx,gx,fx_1p,fx_1q = genrandkey(p, q)
    #    except :
    #        continue
    #    break
    print("p,q is ", p, q)
    fx,gx,fx_1p,fx_1q = genrandkey(p, q)
    hx = genpk(fx_1q, gx, q)

    print("private keys is ",fx.all_coeffs(),fx_1q.all_coeffs())
    return p, q, fx, gx, fx_1p, fx_1q, hx

def p_hx():
    test       = 50
    invertible = 0
    φ1         = Poly(x - 1)
    φ2         = div(R, φ1)[0]
    print(φ1, φ2)
    for i in range(test):
        invertible = invertible + 1
        p, q, fx, gx, fx_1p, fx_1q, hx = initial()
        try:
            hx_1 = invert_poly(hx, φ2, q)
            #hx_1mod2 = invert(hx, φ2, domain=GF(2))
        except NotInvertible :
            invertible = invertible - 1
    print("hx is invertable is ", invertible / test)            
    #get the P is approximately 59% that hx is invertible in φ2

m = Poly(list(map(int, list("101011"))),x)
print(m, m.eval(1))
#normal and bd NTRU
p, q, fx, gx, fx_1p, fx_1q, hx = initial()
c_normal = ntru()
c_bd     = bdntru(c_normal)
detect2(p, q, hx, c_normal, m)
detect2(p, q, hx, c_bd, m)
#detect1(p, q, c_bd, m)
#p_hx()
'''
t1   = 0
t2   = 0
test = 50
for i in range(test):
    p, q, fx, gx, fx_1p, fx_1q, hx = initial()
    c_normal = ntru()
    c_bd     = bdntru(c_normal)

    if detect1(p, q, c_normal, m) == 0:
        t1 = t1 + 1
    if detect1(p, q, c_bd, m) != 0:
        t2 = t2 + 1
print("normal is ",t1 / test * 100, "%" )
print("back door is ",t2 / test * 100, "%" )
'''