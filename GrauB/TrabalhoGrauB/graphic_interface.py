import cv2 as cv
import tkinter
import tkinter.filedialog
from PIL import ImageTk, Image
import filters


class graphic_interface:
    def __init__(self, window_name):
        self.window = tkinter.Tk()
        self.window.title(window_name)
        self.original_image_label = tkinter.Label(self.window, image="", compound="right")
        self.left_button_frame = tkinter.Frame(self.window)
        self.right_button_frame = tkinter.Frame(self.window)
        self.buttons = []

    def load_image(self):
        self.original_image = cv.imread(tkinter.filedialog.askopenfilename()) 
        image = self.convert_cv_to_tk(self.original_image)
        self.display_tk_image(image)

    def webcam_image(self):
        image = cv.imread("GrauB/TrabalhoGrauB/imgs/Webcam.png")
        image = cv.resize(image, (400,400))
        image = self.convert_cv_to_tk(image)
        self.display_tk_image(image)

    def load_webcam(self):
        if hasattr(self, 'original_image'):
            del(self.original_image)
        self.webcam_image()

    def convert_cv_to_tk(self, image):
        image = cv.cvtColor(image, cv.COLOR_BGR2RGB) #converts the img to RGB (format used in tkinter)
        image = Image.fromarray(image)
        return ImageTk.PhotoImage(image)

    def display_tk_image(self, image):
        self.original_image_label.image = image
        self.original_image_label["image"] = image

    def add_left_button(self, button_text, button_function):
        button = tkinter.Button(self.left_button_frame, text=button_text, command=button_function, compound="left")
        self.buttons.append(button)

    def add_right_button(self, button_text, button_function):
        button = tkinter.Button(self.right_button_frame, text=button_text, command=button_function, compound="left")
        self.buttons.append(button)
    
    def display(self):
        for button in self.buttons:
            button.grid()
        self.left_button_frame.grid(column=0, row=0, sticky='nsew')
        self.right_button_frame.grid(column=2, row=0, sticky='nsew')
        self.original_image_label.grid(column=1, row=0,sticky='nsew')
        self.webcam_image()
        self.window.mainloop()

    def select_sticker(self, selected_sticker):
        self.cv_image.sticker = selected_sticker()

    def save_image(self):
        cv.imwrite('GrauB/TrabalhoGrauB/saved_image.png', self.cv_image.filtered_image)

    def create_filter_window(self, filter):
        self.cv_image = image_displayer(filter)

        if hasattr(self, 'original_image'):
            self.cv_image.display_image(self.original_image)
        else:
            self.cv_image.webcam()

class image_displayer:

    def __init__(self, filter):
        self.filter = filter
        self.sticker = cv.imread("GrauB/TrabalhoGrauB/imgs/Stickers/eyeglasses.png", cv.IMREAD_UNCHANGED)

    def display_image(self, image):
        self.image = image
        self.filtered_image = self.filter(image)
        cv.imshow("image", self.filtered_image)
        cv.setMouseCallback("image", self.mouse_click)
        
    def mouse_click(self, event, x, y, 
                    flags, param):

        if event == cv.EVENT_LBUTTONDOWN:
            self.apply_sticker(x, y)
            cv.imshow('image', self.filtered_image)

    def apply_sticker(self, position_x, position_y):

        sticker = cv.cvtColor(self.sticker, cv.COLOR_RGBA2RGB)
        b, g, r, a = cv.split(self.sticker)

        f_rows, f_cols, _ = self.sticker.shape
        b_rows, b_cols, _ = self.filtered_image.shape

        if position_x is None:
            position_x = b_cols // 2
        if position_y is None:
            position_y = b_rows // 2

        x_start = position_x - f_cols // 2
        y_start = position_y - f_rows // 2

        bg_x_start = max(0, x_start)
        bg_y_start = max(0, y_start)
        bg_x_end = min(b_cols, x_start + f_cols)
        bg_y_end = min(b_rows, y_start + f_rows)

        fg_x_start = max(0, -x_start)
        fg_y_start = max(0, -y_start)
        fg_x_end = fg_x_start + (bg_x_end - bg_x_start)
        fg_y_end = fg_y_start + (bg_y_end - bg_y_start)

        sticker = sticker[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
        mask = a[fg_y_start:fg_y_end, fg_x_start:fg_x_end]
        mask_inv = cv.bitwise_not(mask)
        roi = self.filtered_image[bg_y_start:bg_y_end, bg_x_start:bg_x_end]

        img_bg = cv.bitwise_and(roi, roi, mask=mask_inv)
        img_fg = cv.bitwise_and(sticker, sticker, mask=mask)
        res = cv.add(img_bg, img_fg)

        self.filtered_image[bg_y_start:bg_y_end, bg_x_start:bg_x_end] = res

    def webcam(self):
        capture = cv.VideoCapture(0)
        if not capture.isOpened():
            print('Unable to open')
            exit(0)
        while True:
            ret, frame = capture.read()
            if frame is None:
                break
            frame = self.filter(frame)
            cv.imshow('image', frame)
            cv.setMouseCallback("image", self.mouse_click)

            if cv.waitKey(1) & 0xFF == ord('s'):
                cv.imwrite("frame.jpg",frame)
            if cv.waitKey(1) & 0xFF == ord('q'):
                break
