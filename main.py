#!/usr/bin/python
import tkinter as tk
from PIL import Image, ImageTk
import os
import pickle
import numpy as np
from module import create_image

class ImageHolder:
    def __init__(self):
        self.ll_corner = np.array([-1,-1], dtype=np.double)
        self.ur_corner = np.array([1,1], dtype=np.double)
        self.max_iter = 200
        self.update_image()

    def get_center(self):
        center = self.ur_corner + self.ll_corner
        center = center/2
        return center

    def get_dims(self):
        dims = self.ur_corner - self.ll_corner
        return dims

    def zoom(self, direction):
        multiplier = 1.2
        center = self.get_center()
        dims = self.get_dims()

        if direction == 1:
            dims = dims/multiplier
        else:
            dims *= multiplier

        ll_corner = center - dims/2
        ur_corner = center + dims/2
        
        self.ll_corner = ll_corner
        self.ur_corner = ur_corner
        self.update_image()

    def shift(self, direction):
        direction = np.array(direction, dtype=np.double)
        multiplier = 0.2

        center = self.get_center()
        dims = self.get_dims()

        center += direction*dims*multiplier

        self.ll_corner = center - dims/2
        self.ur_corner = center + dims/2
        self.update_image()

    def update_image(self):
        self.image = create_image(128, 128, tuple(self.ll_corner), tuple(self.ur_corner), self.max_iter)
        self.image = self.image.astype(np.double).T
        self.image /= np.max(self.image)/255
        self.image = self.image.astype(np.uint8)
        

class app(tk.Frame):
    def __init__(self, master=None):
        self.imageholder = ImageHolder()
        tk.Frame.__init__(self, master)
        self.pack()

        #self.bind('1', self.b1)
        #self.bind('2', self.b2)
        #self.bind('3', self.b3)

        self.focus_set()

        subf = tk.Frame(self)
        subf.pack()

        qt = tk.Button(subf, text='l', command=lambda: self.move([-1,0]))
        qt.pack(side='left')

        res = tk.Button(subf, text='r', command=lambda: self.move([1,0]))
        res.pack(side='left')

        qt = tk.Button(subf, text='u', command=lambda: self.move([0,-1]))
        qt.pack(side='left')

        res = tk.Button(subf, text='d', command=lambda: self.move([0,1]))
        res.pack(side='left')

        qt = tk.Button(subf, text='i', command=lambda: self.zoom(1))
        qt.pack(side='right')

        res = tk.Button(subf, text='o', command=lambda: self.zoom(-1))
        res.pack(side='right')

        qt = tk.Button(subf, text='incr', command=lambda: self.incr())
        qt.pack(side='right')

        res = tk.Button(subf, text='decr', command=lambda: self.decr())
        res.pack(side='right')

        self.reset_im_frame()

    def incr(self):
        self.imageholder.max_iter += 100
        self.reset_im_frame()

    def decr(self):
        self.imageholder.max_iter -= 100
        self.imageholder.max_iter = max(self.imageholder.max_iter, 50)
        self.reset_im_frame()

    def move(self, direction):
        self.imageholder.shift(direction)
        self.reset_im_frame()

    def zoom(self, direction):
        self.imageholder.zoom(direction)
        self.reset_im_frame()
        
    def reset_im_frame(self):
        if hasattr(self, 'im_frame'):
            self.im_frame.destroy()

        im_frame = tk.Frame(self)
        im_frame.pack(side='bottom', fill='both', expand='yes')
        self.im_frame = im_frame
        self.show_image()

    def show_image(self):
        image = Image.fromarray(self.imageholder.image)
        image = image.resize((800,800), Image.ANTIALIAS)
        photo = ImageTk.PhotoImage(image)
        self.photo = photo

        image = tk.Label(self.im_frame, image=photo)
        image.pack()

    def b1(self, event):
        answers[users[self.i]] = 1
        self.i += 1
        if self.handle_oob():
            return
        self.reset_im_frame()
    

root = tk.Tk()
a = app(root)
a.mainloop()
