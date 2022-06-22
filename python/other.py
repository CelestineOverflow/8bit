from enum import Enum
from re import A
import sys
from turtle import delay
from unittest import result
from webbrowser import get
from numpy import array
import spotipy
import spotipy.util as util
import time
import serial as comunication
import re
arduino = comunication.Serial(port='COM8', baudrate=115200, timeout=.1)
scope = 'user-read-currently-playing', 'user-read-playback-state', 'user-modify-playback-state', 
username = '12174615660'
SPOTIPY_CLIENT_ID = '9324602b14d14d92a50a8add58db4919'
SPOTIPY_CLIENT_SECRET = '4d7f3c5967dd402fb51ffefe00154ff0'

token = util.prompt_for_user_token(username, scope, SPOTIPY_CLIENT_ID, SPOTIPY_CLIENT_SECRET, redirect_uri='http://localhost:8888/callback')

def sense_press():
    print("sense press")
    data = arduino.readline().decode('utf-8')
    print(data)
    arduino.flushInput()
    arduino.flushOutput()
    arduino.sendBreak()
    data.split()
    if data.isdigit() != True:
        data = '0'	
    output = int(data)
    return output

def sendSong(song):
    arduino.write(song.encode('utf-8'))
    print("song sent")
def receiveData():
    data = arduino.readline().decode('utf-8')
    print("song received")
    print(data)

def keyboard_input_from_controller(input):
    input = (int) (input)
    if(input == 1):
        spotipy.Spotify(auth=token).start_playback()
    elif(input == 2):
        spotipy.Spotify(auth=token).pause_playback()
    elif(input == 3):
        spotipy.Spotify(auth=token).next_track()
    elif(input == 4):
        spotipy.Spotify(auth=token).previous_track()
    else:
        print("dont do shit")
        
if token:
    sp = spotipy.Spotify(auth=token),
    songName = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('name')
    album = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('album').get('name')
    artist = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('artists')[0].get('name')
    print(songName + ' - ' + album + ' - ' + artist)
    sendSong("_" + songName.upper().replace(" ", "_") + "_")
    receiveData()
while True:
    time.sleep(1)
    try:
        input = sense_press()
        keyboard_input_from_controller(input)
        songName = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('name')
                
        songName = re.sub('[!\"#$%&\'()*+,-./{|}~]', '', songName)  # remove chars that are outside of the range of ASCII 48-122
        songName = re.sub('[ ]', '_', songName)                     # replace spaces with underscore
        songName = songName.upper()                                 # to uppercase
        sendSong("_" + songName + "_")
        
        receiveData()
    except:
        print("no song playing")
   

