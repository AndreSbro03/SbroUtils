voti = {
    "analisi" :     (23,10),
    "geometria":    (27,8),
    "informatica":  (26,10),
    "elettrotec.":  (30,10),
    "reti":         (27,10),
    "fisica":       (29,12),
    "ACSO":         (29,10),
    "Chimica":      (30,5),
    # "analisi 2":    (24,10),
    # "Logica":       (24,5),
}

somma_voti_ponderati = 0
somma_crediti = 0
for materia in voti:
    somma_voti_ponderati += voti[materia][0] * voti[materia][1]
    somma_crediti += voti[materia][1]

media = somma_voti_ponderati / somma_crediti

print(media)