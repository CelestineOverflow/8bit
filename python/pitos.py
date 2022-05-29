from cmath import sin
words = list("8=================3")
character = 'M'
for i in range(1, 40):
    indexComplex = (sin(i * 0.5) * 5) + 10
    index = int(indexComplex.real)
    output = words.copy()
    output.insert(index, character)
    outputString = "".join(output)
    print(outputString)