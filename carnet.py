## Codice realizzato da Andrea Sbrogiò (andrea.sbrogio.dev@gmail.com)
## L'algoritmo ha l'obbiettivo di trovare il miglior gionro in cui comprare un carnet da 30 giorni ipotizzando un uso bisettimanale il luendì ed il venerdì.
## L'algoritmo carica due mesi, quello con la data corrente e quello con la data di termine del carnet, elimina le settimane non utili. 
## Rimuove poi ognuna delle settimane in mezzo a quelle delle date iniziali e finali aggiungendo 2 biglietti al contatore per ogni settimana rimossa.
## Alla fine controlla i giorni rimanenti per vedere se sono presenti lunedì o venerdì e aggiorna il contatore.

import calendar
import time

# Cerca il giorno  nel primo o nel secondo mese (first != 0) e restituisce
# l'index della settimana in cui si trova, in caso di errore restituisce -1
def getWeek(giorno, monthMatrix, _first):
    first = _first
    out = -1
    for i in range(len(monthMatrix)):
        found = 1
        try:
            monthMatrix[i].index(giorno)
        except:
            found = 0
        if found:
            out = i
            if first == 0:
                return i
            else:
                first = 0

    return out

trys = 7
maxVals = []
allGoods = [[]]
maxBill = -1

#MANUALE
giorno_inp = int(input("Giorno: "))
mese_inp = int(input("Mese: "))

for k in range(trys):

    corrBill = 0

    # MANUALE
    giorno = giorno_inp + k
    mese = mese_inp

    #AUTOMATICO
    #giorno = int(time.strftime("%d")) + k
    #mese = int(time.strftime("%m"))
    anno = 2000 + int(time.strftime("%y"))

    if giorno > calendar.monthrange(anno, mese)[1]:
        giorno = 1
        mese += 1

    if mese > 12:
        mese = 1
        anno += 1

    month = calendar.monthcalendar(anno, mese)

    if mese < 12:
        month_2 = calendar.monthcalendar(anno, mese + 1)
    elif mese == 12:
        month_2 = calendar.monthcalendar(anno + 1, 1)

    #Unione settimana comunicante
    found = 0
    for i in range(7):
        if month[len(month) - 1][i] == 0:
            month[len(month) - 1][i] = month_2[0][i]
            found = 1
    if found:
        month_2.pop(0)

    #Unione mesi
    for i in range(len(month_2)):
        month.append(month_2[i])

    for _ in range(getWeek(giorno, month, 0)):
        month.pop(0)

    lastday = (30 - calendar.monthrange(anno, mese)[1]) + giorno
    if lastday == 0:
        lastday = calendar.monthrange(anno, mese)[1]

    #print(giorno)
    #print(lastday)
    ldWeek = getWeek(lastday, month, 1)
    for _ in range(len(month) - ldWeek - 1):
        month.pop(len(month) - 1)

    #print(month)

    while(len(month) > 2):
        month.pop(1)
        corrBill += 2

    for i in range(7):
        if month[0][i] > giorno:
            if i == 0 or i == 4:
                #print(month[0][i])
                corrBill += 1

    for i in range(7):
        if month[1][i] <= lastday and month[1][i] != 0:
            if i == 0 or i == 4:
                #print(month[1][i])
                corrBill += 1

    if(corrBill > maxBill):
        maxBill = corrBill
        maxVals = [giorno, mese, anno]
        allGoods.clear()

    if corrBill >= maxBill:
        allGoods.append([giorno,mese,anno])

lastday = (30 - calendar.monthrange(maxVals[2], maxVals[1])[1]) + maxVals[0]
if lastday == 0:
    lastday = calendar.monthrange(maxVals[2], maxVals[1])[1]


print("Il giorno migliore per comprare il biglietto è il " +
      str(maxVals[0]) + "/" +
      str(maxVals[1]) + "/" +
      str(maxVals[2]) +
      " in cui potresti comprare " + str(maxBill) + " biglietti. \n" + "Scadrebbe il " + str(lastday))

print("In alternativa i giorni con gli stessi risulatati sono: ", str(allGoods))
