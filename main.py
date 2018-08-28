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
        self.update_image()

    def get_center(self):
        center = self.ur_corner + self.ll_corner
        center = center/2
        return center

    def get_dims(self):
        dims = self.ur_corner - self.ll_corner
        return dims

    def zoom(self):
        multiplier = 1.2
        center = self.get_center()
        dims = self.get_dims()
        dims = dims/multiplier

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
        self.image = create_image(400, 400, tuple(self.ll_corner), tuple(self.ur_corner), 200)
        

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

        qt = tk.Button(subf, text='l', command=lambda x: self.move(x, [-1,0]))
        qt.pack(side='left')

        res = tk.Button(subf, text='r', command=lambda x: self.move(x, [1,0]))
        res.pack(side='left')

        qt = tk.Button(subf, text='u', command=lambda x: self.move(x, [0,1]))
        qt.pack(side='left')

        res = tk.Button(subf, text='d', command=lambda x: self.move(x, [0,-1]))
        res.pack(side='left')

        self.reset_im_frame()

    def move(self, event, direction):
        self.imageholder.shift(direction)
        
    def reset_im_frame(self):
        if hasattr(self, 'im_frame'):
            self.im_frame.destroy()

        im_frame = tk.Frame(self)
        im_frame.pack(side='bottom', fill='both', expand='yes')
        self.im_frame = im_frame
        self.photos = []
        for image in images[users[self.i]]:
            self.show_image(os.path.join(users[self.i], image))

    def show_image(self):
        img = Image.fromarray(self.imageholder.image)
        photo = ImageTk.PhotoImage(image)

        image = tk.Label(self.im_frame, image=photo)
        image.grid(column=0, row=0)

    def b1(self, event):
        answers[users[self.i]] = 1
        self.i += 1
        if self.handle_oob():
            return
        self.reset_im_frame()
    

root = tk.Tk()
a = app(root)
a.mainloop()
