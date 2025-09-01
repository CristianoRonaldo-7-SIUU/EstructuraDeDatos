class GOAT:
    def __init__(self, nombre, edad):
        self.nombre = nombre
        self.edad = edad

goats = [
    GOAT("Ronaldo", 39),
    GOAT("Mourinho", 61),
    GOAT("Khabib", 36)
]

for g in goats:
    print(g.nombre, g.edad)