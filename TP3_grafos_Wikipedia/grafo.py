import random

class Grafo:
    def __init__(self, dirigido=True, pesado=False):
        self.vertices = {}
        self.dirigido = dirigido
        self.pesado = pesado
        # grafo = { V0: {V1: 1, V5: 1}, V2: {V3: 1}, V1: {V2: 1} , V5: {V2:1, V4:1}, V3: {V4:1, V5: 1}, V4: {V0: 1}}
    
    def agregar_vertice(self, v):
        self.vertices[v] = {}

    def borrar_vertice(self, v):
        for vertice in self.vertices:
            if v in self.vertices[vertice]:
                self.vertices[vertice].pop(v)
        if v in self.vertices:
            self.vertices.pop(v)

    def agregar_arista(self, v, w, peso=1):
        if not self.pesado:
            peso = 1
        if v in self.vertices and w in self.vertices:
            self.vertices[v][w] = peso
            if not self.dirigido:
                self.vertices[w][v] = peso

    def borrar_arista(self, v, w):
        if v in self.vertices and w in self.vertices[v]:
            self.vertices[v].pop(w)
            if not self.dirigido:
                self.vertices[w].pop(v)

    def estan_unidos(self, v, w):
        return v in self.vertices and w in self.vertices[v]
        
    def obtener_vertices(self):
        return self.vertices.keys()

    def vertice_aleatorio(self):
        vertices = self.obtener_vertices()
        aleatorio = random.randint(0, len(vertices) - 1)
        for i, v in enumerate(vertices):
            if i == aleatorio:
                return v

    def adyacentes(self, v):
        adyacentes = []
        if v in self.vertices:
            for ady in self.vertices[v]:
                adyacentes.append(ady)
        return adyacentes


    def peso_arista(self, v, w):
        if not self.pesado:
            raise ValueError("El grafo no es pesado")
        if self.estan_unidos(v, w):
            return self.vertices[v][w]
        return None


    def __str__(self):
        return f'{self.vertices}'

    def __iter__(self):
        for v in self.vertices:
            yield v

    def __len__(self):
        return len(self.vertices)
