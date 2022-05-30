from enum import Enum
from re import A
import sys
from turtle import delay
from unittest import result
from webbrowser import get
import spotipy
import spotipy.util as util
import time
import serial as comunication
arduino = comunication.Serial(port='COM11', baudrate=115200, timeout=.1)
scope = 'user-read-currently-playing', 'user-read-playback-state', 'user-modify-playback-state', 
username = '12174615660'
SPOTIPY_CLIENT_ID = '9324602b14d14d92a50a8add58db4919'
SPOTIPY_CLIENT_SECRET = '4d7f3c5967dd402fb51ffefe00154ff0'

token = util.prompt_for_user_token(username, scope, SPOTIPY_CLIENT_ID, SPOTIPY_CLIENT_SECRET, redirect_uri='http://localhost:8888/callback')

def sense_press():
    data = arduino.readline().decode('utf-8')
    print(data)
    arduino.flushInput()
    arduino.flushOutput()
    arduino.sendBreak()
    data.split()
    if data == '':
        data = '9'	
    output = int(data)
    return output

def keyboard_input_from_controller(input):
    if(input == 1):
        spotipy.Spotify(auth=token).start_playback()
    elif(input == 0):
        spotipy.Spotify(auth=token).pause_playback()
    elif(input == 2):
        spotipy.Spotify(auth=token).next_track()
    elif(input == 3):
        spotipy.Spotify(auth=token).previous_track()
        
if token:
    sp = spotipy.Spotify(auth=token),
    songName = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('name')
    album = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('album').get('name')
    artist = spotipy.Spotify(auth=token).current_user_playing_track().get('item').get('artists')[0].get('name')
    print(songName + ' - ' + album + ' - ' + artist)
    #play a song
    #spotipy.Spotify(auth=token).start_playback(uris=['spotify:track:4uLU6hMCjMI75M1A2tKUQC'])
    #pause a song
    #time.sleep(5)
    #spotipy.Spotify(auth=token).pause_playback()
    while(True):
        keyboard_input_from_controller(sense_press())
else:
            print("Can't get token for", username)

