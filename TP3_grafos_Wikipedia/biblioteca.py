from collections import deque

def bfs(grafo, origen): # COMPLEJIDAD --> O(V + E)
    visitados = set()
    padres = {}
    orden = {}
    padres[origen] = None
    orden[origen] = 0
    visitados.add(origen)
    q = deque()
    q.append(origen)
    while q:
        v = q.popleft()
        for w in grafo.adyacentes(v): 
            if w not in visitados:
                padres[w] = v
                orden[w] = orden[v] + 1
                visitados.add(w)
                q.append(w)
    return padres, orden

def grados_entrada(grafo):
    g_ent = {}
    for v in grafo:
        g_ent[v] = 0
    for v in grafo:
        for w in grafo.adyacentes(v):
            g_ent[w] += 1
    return g_ent

def topologico_grados(grafo):
    g_ent = grados_entrada(grafo)
    q = deque()

    for v in grafo:
        if g_ent[v] == 0:
            q.append(v)
    resultado = []

    while len(q) > 0:
        v = q.popleft()
        resultado.append(v)
        for w in grafo.adyacentes(v):
            g_ent[w] -= 1
            if g_ent[w] == 0:
                q.append(w)

    if len(resultado) != len(grafo):
        return None # HAY UN CICLO
    return resultado
