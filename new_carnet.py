# Date una serie di caratteristiche di un carnet (durata, giorni di utilizzo settimanali, giorni in cui non si può usare ...)
# vogliamo trovare il giorno migliore in cui comprarlo per massimizzare il numero di giorni in cui lo si può usare. 
# L'algoritmo ritorna nell'arco di giorni richiesti (default 7) precedenti al giorno da cui si vuole essere certi di avere il carnet,
# il numero di biglietti utilizzabili in base al giorno di acquisto.

from datetime import date, timedelta

class CarnetResult:
    def __init__(self, buyDay, endingDay, rides, avgCost):
        self.buyDay = buyDay
        self.endingDay = endingDay
        self.rides = rides
        self.avgCost = avgCost


class Carnet:
    def __init__(self, duration, cost, startingDate, regularDays=[0, 4]):
        self.duration = duration
        self.cost = cost
        self.startingDate = startingDate
        self.regularDays = regularDays # default [0, 4] = Monday and Friday

    # Given a date go to the next specific weekday. If the date is the same weekday, go to the next one
    def goToNextDay(self, d, day):
        weekDay = d.weekday()
        diff = day - weekDay 
        if diff <= 0:
            diff += 7

        return d + timedelta(days=diff)
    
    def search(self, trys=7):

        results = []

        for n in range(trys):
            rides = 0
            
            # Get the first day: N days before the starting date   
            firstDay = self.startingDate - timedelta(days=n) 
            
            # Get the end day: carnet duration days after the first day
            endDay = firstDay + timedelta(days=self.duration - 1)

            # Check if the starting day is a regular day
            for day in self.regularDays:
                if self.startingDate.weekday() == day:
                    rides += 1

            # From the starting day, jump to the next regular day
            for day in self.regularDays:
                curr = self.goToNextDay(self.startingDate, day)
                while curr <= endDay:
                    rides += 1
                    curr = self.goToNextDay(curr, day)

            avgCost = self.cost / rides if rides > 0 else float('inf')
            results.append(CarnetResult(firstDay, endDay, rides, avgCost))

        # Return an array with the day and the counter and the avarege cost per ride
        return results

if __name__ == "__main__":
    c = Carnet(30, 99.0, date.today())
    c.search() 