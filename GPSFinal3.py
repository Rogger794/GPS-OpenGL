#librerias para importar
#latutud 0 90
#longitud 0 180

import numpy as np
from math import fabs,sqrt,cos,sin,tan,pi,asin,acos,atan
from sys import exit
import csv
import types
#variables globales
A=4#6378.2064 
B=4#6356.5838
def inputMatrix():
  """Ingreso de datos de la matriz"""

  nf = int(input("Ingrese el numero de filas de la matriz: "))
  nc = int(input("Ingrese el numero de columnas de la matriz: "))
  print "Ingrese los elementos de la matriz A fila por fila con un espacio luego enter"
  print "Ejemplo para una matriz de orden 3:","\n", "3 5 1","\n","6 7 9","\n","2 7 1","\n"
  A = [[0.0]*nc for i in range(nf)]
  aux = [[0.0]*nc for i in range(nf)]
  for i in range(nf):
    temp = raw_input()
    A[i] = temp.split()
    for j in range(nc):
      A[i][j] = float(A[i][j])
  
  if A == aux:
    exit('Matriz nula, vuelva a escribir la matriz')
  return A, nf,nc		

def inputVector(M):
  """Ingreso de datos del vector independiente"""

  n = len(M)
  print 'Ingrese los elementos del vector b'
  b = [0]*n
  temp = raw_input();
  b = temp.split()
  for i in range(n):
    b[i] = float(b[i])

  return b

def augmentedMatrix(M, b):
  """Retorna la matriz aumentada"""
  a = [i[:] for i in M]
  for i in range(len(M)):
    a[i].append(b[i])

  return a

def augmentedMatrix2(M, I):
  """Retorna la matriz aumentada para la inversa"""
  a = [i[:] for i in M]
  for i in range(len(M)):
    for j in range(len(I)):
      a[i].append(I[i][j])
  return a

def solMatrixSup(M, b):
  """Resuelve la matriz superior con el metodo inverso"""
  n=len(M)
  m=len(M[0])
  x =[]
  if m<n:
    n=m
  for i in range(n-1,-1,-1):
    x.append((b[i]-sum(M[i][n-j-1]*x[j] for j in range(len(x))))/M[i][i])
  x.reverse()	
  return x

def solMatrixInf(M, b):
  """Resuelve la matriz inferior con el metodo inverso"""
  x = []
  for i in range(len(M)):
    x.append((1.0/(M[i][i]))*(b[i]-sum(M[i][j]*x[j] for j in range(len(x)))))
  return x

def maxColum(M, c):
  """Devuelve la fila del mayor valor debajo de la diagonal en la columna c"""

  r = c #fila
  maximum = M[c][c]
  for i in range(c+1,len(M)):
    if(fabs(maximum) < fabs(M[i][c])):
      maximum = M[i][c]
      r = i

  return r

def trans(M):
  """Calcula la matriz transpuesta de M"""
  nf = len(M)
  nc = len(M[0])
  return [[ M[i][j] for i in range(nf)] for j in range(nc)]

def exchangeRows(M, r1, r2):
  """Intercambia las filas r1 y r2 de M"""
  M[r1], M[r2] = M[r2], M[r1]
  return M

def exchangeCols(M, c1, c2):
  """Intercambia las columnas c1 y c2 de M"""
  for k in range(len(M)):
    M[k][c1] , M[k][c2] = M[k][c2], M[k][c1]
  return M	

def pivotP(M, r1, r2):
  """Permuta la fila r1 con la fila r2 de la matriz M"""
  m = [i[:] for i in M]
  return exchangeRows(m, r1, r2)

def pivotQ(M, r1, r2):
  """Permuta la columna r1 con la columna r2 de la matriz M"""
  return exchangeCols(M, r1, r2)

def Pr(n, r1, r2):
  """Calcula la matriz de permutacion de fila"""
  #Matriz identidad
  I = [[float(i == j) for j in range(n)] for i in range(n)]
  return exchangeRows(I, r1, r2)

def matrixMulti(A, B):
  """Multiplica dos matrices, C = A*B """
  redo=6
  rowsA, colsA = len(A), len(A[0])
  rowsB, colsB = len(B), len(B[0])

  if colsA != rowsB:
    exit('Dimensiones incorrectas')

  C = [[0 for row in range(colsB)] for col in range(rowsA)]

  for i in range(rowsA):
    for j in range(colsB):
      for k in range(colsA):
        C[i][j] += round(A[i][k]*B[k][j],redo)

  return C

def printMatrix(M):
  """Imprime la matriz de una forma legible."""
  for i in range(len(M)):
    print '|',
    for j in range(len(M[i])):
      if(j == len(M)):
        print '|',
        print '{0:13.6f}'.format(M[i][j]),
      else:
        print '{0:13.6f}'.format(M[i][j]),	
    print '|'
  print  

def printMultiMatrices(A, B, C):
  """Imprime la Multiplicacion de dos matrices, A*B=C """
  rowsA, colsA = len(A), len(A[0])
  rowsB, colsB = len(B), len(B[0])
  rowsC, colsC = len(C), len(C[0])
  num=rowsA
  if(rowsA<rowsB):
    num=rowsB
  for i in range(num):
    if i<rowsA:
      print '|',
      for j in range(colsA):
        print '{0:8.4f}'.format(A[i][j]),
    else:
      print ' ',
      for j in range(colsA):
        print '{0:8}'.format(" "),
    print '|',
    if i<rowsB:
      for j in range(colsB):
        print '{0:8.4f}'.format(B[i][j]),
    else:
      for j in range(colsB):
        print '{0:8}'.format(" "),
    if i<=rowsB:
      print '|',
    else:
      print ' ',
    if i==0 and j==(colsB-1):
      print '=',
    else:
      print ' ',
    if i<rowsC:
      print '|',
      for j in range(colsC):
        print '{0:8.4f}'.format(C[i][j]),
    else:
      print ' ',
      for j in range(colsC):
        print '{0:8}'.format(" "),
    if i<rowsC:
      print '|'
    else:
      print 
  print

def maxSubMatrix(M, c):
  """Retorna el mayor elemento de la submatriz A[c]"""
  row = c
  colum = c
  n = len(M)
  maximum = M[c][c]	
  for j in range(c, n):
    for k in range(c, n):
      maxTemp = M[k][j]
      if(fabs(maximum) < fabs(maxTemp)):
        maximum = maxTemp
        colum = j
        row = k
  return row, colum

def pivotT(M, i):
  """Busca el mayor elemento de la submatriz A[i] y permuta filas y columnas"""
  r, c = maxSubMatrix(M, i)
  m = pivotP(M, i, r)		
  return exchangeCols(m, c, i)

def pivot(a, aux, P, Q, colum, v, piv=0):
  """Se encarga del pivoteo en cualquier metodo."""
  if piv > 2 or piv < 0:
    exit('Valores invalidos para el parametro pivoteo, valores validos: 0, 1, 2.')
  n = len(a)	
  temp = a[colum][colum]
  if(temp == 0.0 and piv == 0):
    row_maxColumn = maxColum(a, colum)
    a = pivotP(a, row_maxColumn, colum)
    P = exchangeRows(P, row_maxColumn, colum)
    #comentado por victor print 'P(%d,%d)' % (row_maxColumn, colum)
    p=Pr(n, row_maxColumn, colum)
    #printMultiMatrices(p,aux,matrixMulti(p,aux))comentado por victor
    #comentado por victor print 'P(%d,%d): b' % (row_maxColumn, colum)
    #printMultiMatrices(p,v,matrixMulti(p,v))comentado por victor
    v=matrixMulti(p,v)
    aux=pivotP(aux, row_maxColumn, colum)
  elif piv == 1:
    row_maxColumn = maxColum(a, colum)
    if row_maxColumn != colum:
      a = pivotP(a, row_maxColumn, colum)
      P = exchangeRows(P, row_maxColumn, colum)
      #comentado por victor print 'P(%d,%d)' % (row_maxColumn, colum)
      p=Pr(n, row_maxColumn, colum)
      #comentado por victor printMultiMatrices(p,aux,matrixMulti(p,aux))
      #comentado por victor print 'P(%d,%d): b' % (row_maxColumn, colum)
      #comentado por victor printMultiMatrices(p,v,matrixMulti(p,v))
      v=matrixMulti(p,v)
      aux=pivotP(aux, row_maxColumn, colum)
  elif piv == 2:
    row, c = maxSubMatrix(a, colum)
    if (row != colum) or (c != colum) :
      a = pivotT(a, colum)
      P = exchangeCols(P, row, colum)
      Q = exchangeCols(Q, colum, c)
      #comentado por victor print 'P(%d,%d):' % (colum, row)
      p=Pr(n, row, colum)
      # coemntadop por victor printMultiMatrices(p,aux,matrixMulti(p,aux))
      #comentado por victor print 'P(%d,%d): b' % (colum, row)
      # coemntadop por victor printMultiMatrices(p,v,matrixMulti(p,v))
      v=matrixMulti(p,v)
      aux=pivotP(aux, row, colum)
      #comentado por victor print 'Q(%d,%d):' % (colum, c)
      q=Pr(n,colum, c)
      # coemntadop por victor printMultiMatrices(aux,q,matrixMulti(aux,q))
      aux=pivotQ(aux, colum, c)
      #a[colum][len(a[0])-1],a[c][len(a[0])-1]=a[c][len(a[0])-1],a[colum][len(a[0])-1]
      #aux = matrixMulti(aux,q)
  return a, aux, P, Q, v#retorna la matriz, la matriz de permutacion de filas y la de columnas.	

def L(n, ri, c):
  """Calcula las matrices Li.
  ri: a[i][c] / a[c][c]
  c : columna c-esima
  """
  #Matriz identidad
  L = [[float(i == j) for j in range(n)] for i in range(n)]
  for k in range(c+1, n):
    L[k][c] = -ri[k]			
  return L

def multi(A,x):
  """Multiplica una matriz con un vector"""
  c=[]
  for i in range(len(A)):
    c.append(sum(A[i][j]*x[j] for j in range(len(A[0]))))
  return c

def reverseSub(a):
  """Se usa para resolver la matriz superior de gauss/gauss-Jordan"""
  n = len(a)
  x = [0]*n
  for j in range(n-1, -1, -1):
    if a[j][j] != 0:#Una manera de evitar una excepcion en matriz singular
      x[j] = (a[j][n] - sum(a[j][k]*x[k] for k in range(j+1, n)))/float(a[j][j])
    else:
      print 'Matriz singular, la ecuacion no tiene solucion o tiene infinitas soluciones .'
      break
  return x

def reverseSub2(a):
  """Se usa para resolver la matriz superior de la inversa de una matriz"""
  n = len(a)
  m = len(a[0])
  x = [0]*n
  x = [[0 for j in range(n)] for i in range(n)]
  for g in range(n,m,1):
    for j in range(n-1, -1, -1):
      if a[j][j] != 0:#Una manera de evitar una excepcion en matriz singular
        x[j][g-n] = (a[j][g] - sum(a[j][k]*x[k][g-n] for k in range(j+1, n)))/float(a[j][j])
      else:
        print 'Matriz singular, la ecuacion no tiene solucion o tiene infinitas soluciones .'
        break
  return x


def gauss(A, b, piv=0):
  """
    Metodo de Gauss

    A: Matriz
    b: vector independiente
    piv: pivoteo
    piv=0 sin pivoteo, piv=1 con pivoteo parcial
    piv=2 con pivoteo total
  """
  a = augmentedMatrix(A, b)
  v=[[i] for i in b]
  aux= [i[:] for i in A]
  n = len(A)
  P = [[float(i == j) for j in range(n)] for i in range(n)]
  Q = [[float(i == j) for j in range(n)] for i in range(n)]
  for j in range(len(a[0])):
    r = [0]*n
    if j < n:
      a, aux, P, Q, v= pivot(a, aux, P, Q, j, v, piv)
      #b=[h[:]a]
    for i in range(j+1, n):	
      #Eliminacion de gauss
      r[i] = round(a[i][j] / float(a[j][j]),6)
      for k in range(j, len(a[0])):
        a[i][k] =round(a[i][k] - r[i]*a[j][k],redo)
    if j < n:
      temp = [0]*n
      if (temp != r):
        #print 'L%d' % (j+1)
        #printMatrix(L(n, r, j))
        l= L(n, r, j)
        #printMultiMatrices(l,aux,matrixMulti(l,aux))
        #print 'L%d: b' % (j+1)
        #printMultiMatrices(l,v,matrixMulti(l,v))
        v=matrixMulti(l,v)
    for i in range(j+1, n):	
      #Eliminacion de gauss
      r[i] = aux[i][j] / float(aux[j][j])
      for k in range(j, len(aux[0])):
        aux[i][k] = aux[i][k] - r[i]*aux[j][k]
  #print 'A|b:'			
  #printMatrix(a)
  if piv == 2:
    return multi(Q, reverseSub(a))
  return reverseSub(a)

def norm(x):
  """Norma euclideana del vector x, ie, ||x||2"""
  return sqrt(sum([x_i**2 for x_i in x]))

def Q_i(Q_min, i, j, k):
  """Rellenando elementos de la matrix Q_t"""
  if i < k or j < k:
    return float(i == j)
  else:
    return Q_min[i-k][j-k]

def Householder(A):
  """ Transformacion por Householder, donde Hn...H2*H1*A = R, Q_t*A = R
      Q = H1*H2 ... Hn, Q es ortogonal (Q * Qt = I)
      Retorna Q y R, donde A = Q*R 
  """
  nf = len(A)
  nc = len(A[0])
  R= [i[:] for i in A]
  Q = [[0.0] * nf for i in range(nf)]

  for k in range(nf-1):
    I = [[float(i == j) for i in range(nf)] for j in range(nf)]

    # Se crea los vectores x, e y un escalar alpha
    x = [row[k] for row in R[k:]]
    e = [row[k] for row in I[k:]]
    #La funcion cmp(a, b) retorna -1 si a<b, 1 si a>b, 0 si a==b
    alpha = -cmp(x[0],0) * norm(x) 

    #print alpha

    #Se crea los vectores u, v
    u = map(lambda p,q: p + alpha * q, x, e)
    norm_u = norm(u)
    #print "norma u",norm_u
    v = map(lambda p: p/norm_u, u)
    #print "v",v
    #Se crea la matriz menor Q_t
    Q_min = [ [float(i==j) - 2.0 * v[i] * v[j] for i in range(nf-k)] for j in range(nf-k) ]

    #Se rellena la matriz menor Q (Q_min)
    Q_t = [[ Q_i(Q_min,i,j,k) for i in range(nf)] for j in range(nf)]

    #printMatrix(Q_t)

    #Si esta en la primera ejecutada, entonces se calcula Q_t*A = R
    #Sino, Q_tn .. Q_t1*A = R
    if k == 0:
      Q = Q_t
      R = matrixMulti(Q_t,A)
      #print "rrr"
      #printMatrix(R)
    else:
      Q = matrixMulti(Q_t,Q)
      R = matrixMulti(Q_t,R)
      #print "rrr"
      #printMatrix(R)

    #Se retorna la transpuesta de los Q_t, ie, trans(Q_tn* ... *Q_t1) = Q
  return trans(Q), R

def GramSchmit(A):
  """ Transformacion por GramSchmit, ortogonalizacion de los vectores columna
      Retorna Q y R, donde A = Q*R 
  """
  redo=6
  nf = len(A)
  nc = len(A[0])
  a = [i[:] for i in A]
  Q = [i[:] for i in A]
  R = [[0.0] * nc for i in range(nc)]
  print Q
  for k in range(nc):

    e1 = [row[k] for row in Q[0:]]
    e3 = [row[k] for row in Q[0:]]
    for i in range(k):
      e2 = [row[i] for row in Q[0:]]
      R[i][k]=round(sum(map(lambda p,q: p*q, e3,e2)),redo)
      e1 = map(lambda p,r: p - R[i][k] * r, e1,e2)
    R[k][k] = norm(e1)
    for j in range(len(Q)):
      Q[j][k]=round(e1[j]/R[k][k],redo)
  return Q,R

def givens(A):
  """	Gn* ... G2*G1*A = R
		Q_t = Gn* ... G2*G1
		A = Q*R, de la propiedad Q_t * Q = I
  """
  n = len(A)
  redo=6
  An = [i[:] for i in A]
  Gn = [[float(i == j) for j in range(n)] for i in range(n)]
  Q_t = [[float(i == j) for j in range(n)] for i in range(n)]

  a = An[0][n-2]
  b = An[0][n-1]
  index = 1
  for i in range(n):
    for j in range(n-1, i, -1):
      a = An[j-1][i]
      b = An[j][i]
      #print "a,b",a,b
      if a*a + b*b == 0:
        continue
      cosX = round(a / (sqrt(a*a + b*b)) ,redo)
      sinX = round(-b / (sqrt(a*a + b*b)),redo)
      #print "a cos, -b sin",cosX,sinX
      Gn[j][j] = cosX
      Gn[j][j-1] = sinX
      Gn[j-1][j] = -sinX
      Gn[j-1][j-1] = cosX
      #print 'G' +str(index) + ':'
      #printMatrix(Gn)
      An = matrixMulti(Gn, An)
      #print 'A' +str(index) + ':'
      #printMatrix(An)
      Q_t = matrixMulti(Gn, Q_t)
      #Volviendo la matriz Gn a la identidad
      Gn = [[float(k == l) for l in range(n)] for k in range(n)]
      index += 1
  return trans(Q_t), An

def cholesky(Mat):
  #if not symmetricMatrix(A):
    #exit('La matriz no es simetrica')
  n = len(Mat)
  A = [i[:] for i in Mat]
  G = [[0.0]*n for j in range(n)]
  for i in range(n):
    suma = A[i][i]
    for k in range(i):
      suma -= A[k][i]**2
    if suma < 0:
      exit('No es definida positiva')	
    A[i][i] = sqrt(suma)
    for j in range(i+1, n):
      suma = A[i][j]
      for k in range(i):
        suma -= A[k][i]*A[k][j]
      A[i][j] = suma / A[i][i]
  for j in range(n):
    for i in range(n):
      if(i > j):
        A[i][j] = 0.0
  return A

def Inversa(A):
  """
    Inversa por metodo de Gauss
  """  
  aux= [i[:] for i in A]
  n = len(A)
  I = [[float(i == j) for j in range(n)] for i in range(n)]
  a = augmentedMatrix2(A, I)
  s=0
  for j in range(len(a[0])):
    r = [0]*n
    for i in range(j+1, n):	
      #Eliminacion de gauss
      if float(a[j][j])==0.0:
        print 'valor igual a cero, no se puede sacar la inversa'
        s=1
        break
      else:
        r[i] = a[i][j] / float(a[j][j])
        for k in range(j, len(a[0])):
          a[i][k] = a[i][k] - r[i]*a[j][k]
    if s!=0:
      break
    if j < n:
      temp = [0]*n
      if (temp != r):
        #comentadp popr victor print 'L%d' % (j+1)
        l= L(n, r, j)
        #comentado por victor printMultiMatrices(l,aux,matrixMulti(l,aux))
        #comentadp popr victor print 'L%d: I' % (j+1)
        #comentado por victor printMultiMatrices(l,I,matrixMulti(l,I))
        I=matrixMulti(l,I)
    for i in range(j+1, n):	
      #Eliminacion de gauss
      r[i] = aux[i][j] / float(aux[j][j])
      for k in range(j, len(aux[0])):
        aux[i][k] = aux[i][k] - r[i]*aux[j][k]
  print 'A|I:'			
  #printMatrix(a)comentadoo por victor
  return reverseSub2(a)

def normaInfVector(L):
  """ Calcula la norma infinita de un vector:
		||x|| = max {|xi|}, i = 0, 1, ... n.
  """
  maximum = fabs(L[0])
  for i in range(1, len(L)):
    maximum = max(maximum, fabs(L[i]))
  return maximum

def jacobi(A, b, prec=1e-6):
  """Metodo que calcula la solucion Ax = b, usando tecnicas iterativas"""
  n = len(A)
  Xk = [0.0]*n
  Xk2 = [0.0]*n
  #Xk = [0.0,1.0,0.0,1.0,0.0,1.0]
  sumation = 0.0
  for i in range(n):
    if A[i][i] == 0:
      exit('Los elementos A[i][i] deben ser diferentes de 0')
  Xk1 = [b[i]/float(A[i][i]) for i in range(n)]
  #Xk1 = [0.0,1.0,0.0,1.0,0.0,1.0]
  minus = lambda x, y: [x[i]-y[i] for i in range(n)]
  for j in range(n):
    dominancia = 0.0
    for i in range(j+1, n):
      if j != i:
        dominancia += fabs(A[i][j])
        if A[i][i] < dominancia:
          exit('La matriz no converge')
  while (normaInfVector(minus(Xk1,Xk)) / float(normaInfVector(Xk1))) > prec:
    Xk[:] = Xk1[:]
    #Xk2[:] = Xk1[:]
    for i in range(n):
      sumation = sum(A[i][j]*Xk1[j] if i!=j else 0 for j in range(n))
      #recuerda cambiar xk1 por xk2
      Xk1[i] = (1.0/A[i][i])*(b[i] - sumation)
      #print Xk1
      #Xk1[:] = Xk2[:]
  return Xk1

def sor(A, b, prec=1e-7, w=1.5):
  """ Metodo SOR, calcula la solucion de un sistema de ecuaciones
		usando el parametro de relajacion w, que necesariamente varia
		de <0, 2> solo en el caso que la matriz converja.
  """
  n = len(A)
  Xk = [0.0]*n
  sumation = 0.0
  #Garantizar que los elementos de la diagonal principal sean distintos de cero
  for i in range(n):
    if A[i][i] == 0:
      exit('Los elementos A[i][i] deben ser diferentes de 0')
  Xk1 = [b[i]/float(A[i][i]) for i in range(n)]
  #Xk1 = [0.0,1.0,0.0,1.0,0.0,1.0]
  minus = lambda x, y: [x[i]-y[i] for i in range(n)]
  while (normaInfVector(minus(Xk1,Xk)) / float(normaInfVector(Xk1))) > prec:
    Xk[:] = Xk1[:]
    for i in range(n):
      sumation1 = sum(A[i][j]*Xk1[j] for j in range(i))
      sumation2 = sum(A[i][j]*Xk1[j] for j in range(i+1, n))
      Xk1[i] = (float(w)/A[i][i])*(b[i] - sumation1 - sumation2) + (1-w)*Xk[i]
    #print Xk1
  return Xk1

#funcion norma con redondeo
def norma(x):
  a=0
  for i in range(len(x)):
    a+=x[i]**2

  return pow(a,0.5)

#funcion determinante con redondeo
def det(x):
  return float(x[0][0]*x[1][1]*x[2][2]+x[1][0]*x[2][1]*x[0][2]+x[2][0]*x[0][1]*x[1][2]-x[0][2]*x[1][1]*x[2][0]-x[1][2]*x[2][1]*x[0][0]-x[2][2]*x[0][1]*x[1][0])

#Bota las coordenada de GPS
def gms(u):
  gu=int(u)
  mu=int((u-gu)*60)
  su=((u-gu)*60-mu)*60
  return str(gu)+" "+str(mu)+" "+str(su)

#Convierte las coordenadas a un numero float
def fullg(v):
  g=v[0]+v[1]/60.0+v[2]/3600.0
  return g

#Convierte las coordenadas GPS a coordenadas cartesianas
def  xyz(v,u):
  redo=6
  x=A*cos((u*pi)/180)*sin((v*pi)/180)
  y=A*cos((u*pi)/180)*cos((v*pi)/180)
  z=B*sin((u*pi)/180)
  c=[x,y,z]
  return c

#Convierte las coordenadas cartesianas a coordenadas GPS
def Latitudlong(x,y,z):
  redo=6
  u=round((asin(z/B)*180)/pi,6)#latitud
  v=round((atan(y/x)*180)/pi,6)#longitud
  ## Cuando x > 0 ZONA ESTE
  if x >= 0 and y < 0:
	v = 90 - np.abs(v)
    #us = str(np.abs(u)) 
  if x >= 0 and y > 0:
    v = 90 + v
  ## Cuando x < 0 ZONA OESTE
  if x <= 0 and y > 0:
    v = 90 + np.abs(v)
  if x <=0 and y < 0:
    v = 90 - v
    #us = str(np.abs(u))
  if z <= 0:
    if x < 0:
      cadena=str(np.abs(u))+'S '+str(v)+'W'
    elif x >= 0:
      cadena=str(np.abs(u)) + 'S ' + str(v) + 'E'	  
  elif z > 0:
    if x < 0:
      cadena=str(u) + 'N ' + str(v) + 'W'
    elif x >= 0:
      cadena=str(u) + 'N ' + str(v) + 'E'
  print("###### " + cadena)
  return cadena


with open('punto.csv') as csvpunto:

  posPUNTO = csv.reader(csvpunto, delimiter=',')
  w=[]
  for row in posPUNTO:
    for i in range(3):
        pXYZ = float(row[i])
        w.append(pXYZ) 
  
with open('satelites.csv') as csvsatelites:

  satelCSV = csv.reader(csvsatelites, delimiter=',')
  satelite1=[]
  satelite2=[]
  satelite3=[]
  satelite4=[]
  for row in satelCSV:
    for i in range(12):
      if(i<3):
        posicionXYZ = float(row[i])
        satelite1.append(posicionXYZ)
      else:
        if(i<6):
          posicionXYZ = float(row[i])
          satelite2.append(posicionXYZ)
        else:
          if(i<9):
            posicionXYZ = float(row[i])
            satelite3.append(posicionXYZ)
          else:
            posicionXYZ = float(row[i])
            satelite4.append(posicionXYZ)

  PR=w[:];
  s=np.array([[satelite1[0],satelite1[1],satelite1[2]],[satelite2[0],satelite2[1],satelite2[2]],[satelite3[0],satelite3[1],satelite3[2]],[satelite4[0],satelite4[1],satelite4[2]]])
  c=1
  redo=6
  #punto real
  punto=np.array([w[0],w[1],w[2]])
  print "punto real"
  print punto
  r=Latitudlong(punto[0],punto[1],punto[2])#Convierte las coordenadas cartesianas a coordenadas GPS
  print "Imprimiendo puntos r: "
  print r
  outfile = open('datosLatLong.csv','wb')
  outfile.write(r+"\n")###Grabando Latitud
  error=[0.0,0.0,0.0]
  #distancia al punto real ///CALCULADA CON LA NORMA s array de satelites victor
  dis=np.array([norma(s[0]-w),norma(s[1]-w),norma(s[2]-w),norma(s[3]-w)])
  #norma satelite
  rs=np.array([norma(s[0]),norma(s[1]),norma(s[2]),norma(s[3])])

  a=round((dis[1]**2-dis[0]**2+rs[0]**2-rs[1]**2)/2,redo)
  b=round((dis[2]**2-dis[1]**2+rs[1]**2-rs[2]**2)/2,redo)
  c=round((dis[3]**2-dis[2]**2+rs[2]**2-rs[3]**2)/2,redo)
  #matriz que sera el denominador en cramer
  mi=np.array([s[0]-s[1],s[1]-s[2],s[2]-s[3]])#de los satelites
  Matrix = [[mi[i][j] for j in range(3)] for i in range(3)]
  re=[]
  re.append(a)
  re.append(b)
  re.append(c)  
  Q,R=GramSchmit(Matrix)
  b_prima = multi(trans(Q), re)
  x=solMatrixSup(R, b_prima)
  print "GramSchimit"
  print x
  q=Latitudlong(x[0],x[1],x[2])
  outfile.write(q+"\n")###Grabando Longitud
  for i in range(len(x)):
    error[i]=PR[i]-x[i]
  Q,R=Householder(Matrix)
  b_prima = multi(trans(Q), re)
  x=solMatrixSup(R, b_prima)
  print "house holder"
  print x
  for i in range(len(x)):
    error[i]=PR[i]-x[i]
  q=Latitudlong(x[0],x[1],x[2])
  outfile.write(q+"\n")###Grabando Latitud
  Q, R = givens(Matrix)
  b_prima = multi(trans(Q), re)
  x=solMatrixSup(R, b_prima)
  print "givens"
  print x
  q=Latitudlong(x[0],x[1],x[2])
  outfile.write(q+"\n")###Grabando Longitud
  print"gauss seidel"
  x=gauss(Matrix,re)
  print x
  q=Latitudlong(x[0],x[1],x[2])
  outfile.write(q+"\n")###Grabando Latitud
  outfile.close()