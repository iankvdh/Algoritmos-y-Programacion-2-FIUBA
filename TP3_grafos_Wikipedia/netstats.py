#!/usr/bin/python3

import csv
from biblioteca import *
from grafo import *
import sys
sys.setrecursionlimit(100000)

def reconstruir_camino(padres, destino):
    '''O(P)'''
    recorrido = []
    while destino is not None:
        recorrido.append(destino)
        destino = padres[destino]
    return recorrido[::-1]

def _lista_a_str(lista_parametros, separador):
    parametros = ""
    for palabra in lista_parametros:
        parametros += palabra + separador
    return parametros.rstrip(separador)


def camino_mas_corto(grafo, origen, destino):
    '''O(P + L)'''
    padres, orden = bfs(grafo, origen) # O(P + L)

    if not origen in padres or not destino in padres: # O(1)
        print("No se encontro recorrido")
        return

    lista_camino = reconstruir_camino(padres, destino) #O(P)

    print(_lista_a_str(lista_camino, " -> "))
    print(f'Costo: {orden[destino]}')


def max_clave_valor(orden):
    valor_max = 0
    clave_max = None
    for clave, valor in orden.items():
        if valor > valor_max:
            valor_max = valor
            clave_max = clave
    return clave_max, valor_max 

def diametro(grafo): 
    '''O(V*(V+E))'''
    distancia_max_min = 0
    padres_max_min  = {}
    clave_max_min = None
    for v in grafo: # O(V)
        padres, orden = bfs(grafo, v) #O(V+E)
        clave, distancia = max_clave_valor(orden) # O(V)
        if distancia > distancia_max_min:
            distancia_max_min = distancia
            padres_max_min = padres
            clave_max_min = clave
    lista_camino = reconstruir_camino(padres_max_min, clave_max_min) # O(V)

    print(_lista_a_str(lista_camino, " -> "))
    print(f'Costo: {distancia_max_min}')


def todos_en_rango(grafo, pagina, n):
    '''O(P + L)'''
    _, orden = bfs(grafo, pagina) # O(P + L)
    total = 0
    for costo in orden.values(): # O(P)
        if costo == n:
            total += 1
    print(total)


def lectura_2_am(grafo, parametros):
    '''O(n²)'''

    grafito = Grafo()

    for p in parametros: #O(n)
        grafito.agregar_vertice(p)

    for p in parametros: #O(n)
        for ady in grafo.adyacentes(p): #O(n)
            grafito.agregar_arista(p, ady)

    orden = topologico_grados(grafito) #(n + Ln)

    if orden:
        print(_lista_a_str(orden[::-1], ", "))
    else:
        print("No existe forma de leer las paginas en orden")


def navegacion(grafo, origen):
    '''
    O(n)
    '''
    adyacentes = grafo.adyacentes(origen)
    resultado = []
    resultado.append(origen)
    for _ in range(20): # O(n)
        if len(adyacentes) > 0:
            resultado.append(adyacentes[0])
            adyacentes = grafo.adyacentes(adyacentes[0])
    
    print(_lista_a_str(resultado, " -> "))


def calcular_clustering(grafo, v):
    '''
    O(L²) de un vertice
    '''
    if len(grafo.adyacentes(v)) < 2:
        return 0
        
    suma = 0
    adyacentes_v = set(grafo.adyacentes(v))
    
    for ady in adyacentes_v: # O(L)
        for ady_de_ady in grafo.adyacentes(ady): # O(L)
            if ady_de_ady in adyacentes_v:
                if ady != v and ady != ady_de_ady:
                    suma += 1
    
    return suma / (len(adyacentes_v) * (len(adyacentes_v) - 1))

def clustering(grafo, pagina):
    '''
    (L² * P) para todo el grafo
    '''
    if pagina:
        print(round(calcular_clustering(grafo, pagina), 3)) 
        return
    
    suma = 0
    for v in grafo:
        suma += calcular_clustering(grafo, v)
    print(round((suma / len(grafo)), 3 ))

def cargar_grafo(archivo):
    g = Grafo()
    with open(archivo,'r') as f:
        f_reader = csv.reader(f,delimiter = '	')

        for linea in f_reader: # O(v)
            if not linea:
                continue
            g.agregar_vertice(linea[0])

            for i, link in enumerate(linea):  # O(v)
                if i == 0: continue
                if not link in g.obtener_vertices(): # O(1)
                    g.agregar_vertice(link) # O(1)
                g.agregar_arista(linea[0], link) # O(1)
    return g

def interaccion(grafo, operaciones, str_operaciones):
    entrada = ""

    while entrada != EOFError:
        entrada = input()
        comando = entrada.split(" ")[0]

        parametros = _lista_a_str(entrada.split(" ")[1::], " ")

        if comando in operaciones:
            if comando == "camino":
                origen, destino = parametros.split(",")
                camino_mas_corto(grafo, origen, destino)

            if comando == "lectura":
                paginas = set(parametros.split(","))
                lectura_2_am(grafo, paginas)

            if comando == "diametro":
                diametro(grafo)

            if comando == "rango":
                pagina, n = parametros.split(",")
                todos_en_rango(grafo, pagina, int(n))

            if comando == "navegacion":
                navegacion(grafo, parametros)

            if comando == "clustering":
                clustering(grafo, parametros)

            if comando == "listar_operaciones":
                print(str_operaciones)

def main():
    wiki = sys.argv[1]
    grafo = cargar_grafo(wiki)
    
    str_operaciones = "camino\ndiametro\nrango\nlectura\nnavegacion\nclustering" # para que sea en O(1) printear todas
    lista = ["camino", "diametro", "rango", "lectura", "navegacion", "clustering", "listar_operaciones"]
    operaciones = set(lista)

    try:
        interaccion(grafo, operaciones, str_operaciones)
    except EOFError:
        return

main()

# VAMOS 8 ESTRELLAS ★★★★★★★★

###############
## YA HECHAS ##
###############
#Camino más corto (★)
#Diametro (★)
#Todos en Rango (★)
#Navegacion por primer link (★)
#Coeficiente de Clustering (★★)
#Lectura a las 2 a.m. (★★)

######################
## FALTAN POR HACER ##
######################
#Conectividad (★★)
#Comunidades (★★)
#Ciclo de n artículos (★★★)
#Artículos más importantes (★★★)
