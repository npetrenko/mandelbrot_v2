#!/usr/bin/python
import tkinter as tk
from PIL import Image, ImageTk
import os
import pickle
import numpy as np
from libmandelbrot import rebuild_image

command_dic = {
        "none" : 0,
        "left" : 2,
        "down" : 4,
        "up" : 1,
        "right" : 3,
        "in" : 5,
        "out" : 6,
        "increase_depth" : 7,
        "decrease_depth" : 8,
        "test_function"  : 100500
}

class ImageHolder:
    def __init__(self):
        self.image_arr_uint16 = np.zeros((128,128), dtype=np.uint16)
        self.update_image()

    def get_center(self):
        pass

    def get_dims(self):
        return self.image_arr_uint16.shape

    def zoom(self, direction):
        pass

    def shift(self, direction):
        rebuild_image(self.image_arr_uint16, command_dic[direction])

    def update_image(self):
        rebuild_image(self.image_arr_uint16, command_dic["none"])
        

class app(tk.Frame):
    def __init__(self, master=None):
        self.imageholder = ImageHolder()
        tk.Frame.__init__(self, master)
        self.pack()

        self.focus_set()

        subf = tk.Frame(self)
        subf.pack()

        qt = tk.Button(subf, text='l', command=lambda: self.move("left"))
        qt.pack(side='left')

        res = tk.Button(subf, text='r', command=lambda: self.move("right"))
        res.pack(side='left')

        qt = tk.Button(subf, text='u', command=lambda: self.move("up"))
        qt.pack(side='left')

        res = tk.Button(subf, text='d', command=lambda: self.move("down"))
        res.pack(side='left')

        res = tk.Button(subf, text='in', command=lambda: self.move("in"))
        res.pack(side='left')
        res = tk.Button(subf, text='out', command=lambda: self.move("out"))
        res.pack(side='left')

        res = tk.Button(subf, text='++d', command=lambda: self.move("increase_depth"))
        res.pack(side='right')
        res = tk.Button(subf, text='--d', command=lambda: self.move("decrease_depth"))
        res.pack(side='right')

        res = tk.Button(subf, text='lowres', command=lambda: self.change_res("low"))
        res.pack(side='right')
        res = tk.Button(subf, text='highres', command=lambda: self.change_res("high"))
        res.pack(side='right')

        self.reset_im_frame()

    def change_res(self, new):
        if (new == "high"):
            new_res = (512,512)
        else:
            new_res = (48, 48)
            
        self.imageholder.image_arr_uint16 = np.zeros(new_res, dtype=np.uint16)
        self.imageholder.shift("none")
        self.reset_im_frame()

    def move(self, direction):
        self.imageholder.shift(direction)
        self.reset_im_frame()
        
    def reset_im_frame(self):
        if hasattr(self, 'im_frame'):
            self.im_frame.destroy()

        im_frame = tk.Frame(self)
        im_frame.pack(side='bottom', fill='both', expand='yes')
        self.im_frame = im_frame
        self.show_image()

    def show_image(self):
        image = self.imageholder.image_arr_uint16.astype(np.double)
        image /= image.max()
        image *= 255
        image = image.astype(np.uint8)
        self.image = image

        image = Image.fromarray(self.image)
        image = image.resize((800,800), Image.ANTIALIAS)
        photo = ImageTk.PhotoImage(image)
        self.photo = photo

        image = tk.Label(self.im_frame, image=photo)
        image.pack()

root = tk.Tk()
a = app(root)
a.mainloop()
