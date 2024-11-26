import cv2 as cv
import tkinter
import tkinter.filedialog
from PIL import ImageTk, Image
import filters
import graphic_interface
import select_sticker



screen = graphic_interface.graphic_interface("Ramiro - App edição de imagens")

screen.add_left_button("local image", screen.load_image)
screen.add_left_button("webcam", screen.load_webcam)
screen.add_left_button("save image", screen.save_image)

#space_between_button = tkinter.Label(button_frame, text=" ", compound="left")
#space_between_button.grid()

screen.add_left_button("orginial image", lambda: screen.create_filter_window(filters.originial_image) )
screen.add_left_button("gray scale", lambda: screen.create_filter_window(filters.gray_scale))
screen.add_left_button("binarized", lambda: screen.create_filter_window(filters.binarized))
screen.add_left_button("inverted", lambda: screen.create_filter_window(filters.inverted))
screen.add_left_button("blue colored", lambda: screen.create_filter_window(filters.blue_colorized))
screen.add_left_button("green colored", lambda: screen.create_filter_window(filters.green_colorized))
screen.add_left_button("red colored", lambda: screen.create_filter_window(filters.red_colorized))
screen.add_left_button("edged", lambda: screen.create_filter_window(filters.edged))
screen.add_left_button("correlated", lambda: screen.create_filter_window(filters.correlation))
screen.add_left_button("max saturation", lambda: screen.create_filter_window(filters.max_saturation))
screen.add_left_button("thermal", lambda: screen.create_filter_window(filters.thermal))

screen.add_right_button("sunglasses", lambda: screen.select_sticker(select_sticker.sun_glasses))
screen.add_right_button("wow", lambda: screen.select_sticker(select_sticker.wow))
screen.add_right_button("whatsapp", lambda: screen.select_sticker(select_sticker.whatsapp))
screen.add_right_button("nerd", lambda: screen.select_sticker(select_sticker.nerd))
screen.add_right_button("top", lambda: screen.select_sticker(select_sticker.top))
screen.add_right_button("joia", lambda: screen.select_sticker(select_sticker.joia))

screen.display()
