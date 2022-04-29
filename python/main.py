# Imports PIL module
from fileinput import filename
from webbrowser import get
from PIL import Image
from datetime import datetime
# nice way to create unique file names
def dateAsString():
    # get current second
    second = datetime.now().second
    # get current minute
    minute = datetime.now().minute
    # get current hour
    hour = datetime.now().hour
    # get current day
    day = datetime.now().day
    # get current month
    month = datetime.now().month
    # get current year
    year = datetime.now().year
    return str(year) + str(month) + str(day) + str(hour) + str(minute) + str(second)
# open method used to open different extension image file
im = Image.open(r"input.png")
print(im.format, im.size, im.mode)
# convert image into RGB mode
im = im.convert("RGB")
# get the image size
width, height = im.size
filename = "output_" + dateAsString() + ".txt"
f = open(filename, "a")
entry_text = "bool characters["+ str(height) + "][" + str(width) + "] = {"
print(entry_text)
f.write(entry_text)
for y in range(height):
    print("\t{", end="")
    f.write("\t{")
    for x in range(width):
        r, g, b = im.getpixel((x, y))
        # calculate the brightness
        brightness = int(r * 0.299 + g * 0.587 + b * 0.114)
        if brightness < 255/2:
            brightness = 1
        else:
            brightness = 0
        print(brightness, end="")
        f.write(str(brightness))
        if(x != width - 1):
            print(",", end="")
            f.write(",")
        else:
            print("", end="")
            f.write("")
    if y != height-1:
        print("},", end="")
        f.write("},")
    else:
        print("}", end="")
        f.write("}")
    # print new line
    print()
    f.write("\n")
print("};")
f.write("};")
im.show()
f.close()
print("The file is saved as output.txt")
print("current time: " + dateAsString())


