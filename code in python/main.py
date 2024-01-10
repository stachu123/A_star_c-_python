import math

r_c, c_c = 1, 5

tablica = [
    [0, 0, 0, 0, 0, 0],
    [0, 5, 0, 0, 0, 0],
    [0, 0, 5, 0, 0, 0],
    [0, 0, 0, 5, 0, 0],
    [0, 0, 0, 0, 5, 0],
    [0, 0, 0, 0, 0, 0]
]

lista_otwarta = []
lista_zamknieta = [[2, 1]]
r_s, c_s = 2, 1

# Initialize parent array
parent = [[None for _ in range(6)] for _ in range(6)]

def oblicz_g(punkt):
    r = punkt[0]
    c = punkt[1]
    path = []
    current = [r, c]
    while current != [r_s, c_s]:
        path.append(current)
        current = parent[current[0]][current[1]]
    path.append([r_s, c_s])
    f_g = len(path)
    h_g = math.sqrt((r - r_c) ** 2 + (c - c_c) ** 2)
    return f_g + h_g

def wybierz_do_zamknietej(lista):
    najmniejsza_wartosc = oblicz_g(lista[0])
    indeks_najmniejszej = 0
    for i in range(1, len(lista)):
        if oblicz_g(lista[i]) <= najmniejsza_wartosc:
            najmniejsza_wartosc = oblicz_g(lista[i])
            indeks_najmniejszej = i
    return indeks_najmniejszej

r, c = 2, 1

while r != r_c or c != c_c:  # Corrected the condition to use 'or'

    if r - 1 >= 0 and tablica[r - 1][c] != 5:
        if ([r - 1, c] not in lista_otwarta) and ([r - 1, c] not in lista_zamknieta):
            lista_otwarta.append([r - 1, c])
            parent[r - 1][c] = [r, c]

    if r + 1 <= 5 and tablica[r + 1][c] != 5:
        if ([r + 1, c] not in lista_otwarta) and ([r + 1, c] not in lista_zamknieta):
            lista_otwarta.append([r + 1, c])
            parent[r + 1][c] = [r, c]

    if c - 1 >= 0 and tablica[r][c - 1] != 5:
        if ([r, c - 1] not in lista_otwarta) and ([r, c - 1] not in lista_zamknieta):
            lista_otwarta.append([r, c - 1])
            parent[r][c - 1] = [r, c]

    if c + 1 <= 5 and tablica[r][c + 1] != 5:
        if ([r, c + 1] not in lista_otwarta) and ([r, c + 1] not in lista_zamknieta):
            lista_otwarta.append([r, c + 1])
            parent[r][c + 1] = [r, c]


    nastepny = lista_otwarta[wybierz_do_zamknietej(lista_otwarta)]
    lista_zamknieta.append(nastepny)
    r, c = nastepny
    lista_otwarta.remove(nastepny)

    if not lista_otwarta:
        break


# Path Reconstruction
path = []
current = [r_c, c_c]
while current != [r_s, c_s]:
    path.append(current)
    current = parent[current[0]][current[1]]
path.append([r_s, c_s])
path.reverse()

print("Path:", path)
