"""
Simple program showing how to use a shape list to create a more complex shape
out of basic ones.

If Python and Arcade are installed, this example can be run from the command line with:
python -m arcade.examples.shape_list_demo_person
"""
import arcade
import NDIImageSender

SCREEN_WIDTH = 512
SCREEN_HEIGHT = 512
SCREEN_TITLE = "Shape List Demo Person"


def make_person(head_radius,
                chest_height,
                chest_width,
                leg_width,
                leg_height,
                arm_width,
                arm_length,
                arm_gap,
                shoulder_height):

    shape_list = arcade.ShapeElementList()

    # Head
    shape = arcade.create_ellipse_filled(0, chest_height / 2 + head_radius, head_radius, head_radius,
                                         arcade.color.WHITE)
    shape_list.append(shape)

    # Chest
    shape = arcade.create_rectangle_filled(0, 0, chest_width, chest_height, arcade.color.BLACK)
    shape_list.append(shape)

    # Left leg
    shape = arcade.create_rectangle_filled(-(chest_width / 2) + leg_width / 2, -(chest_height / 2) - leg_height / 2,
                                           leg_width, leg_height, arcade.color.RED)
    shape_list.append(shape)

    # Right leg
    shape = arcade.create_rectangle_filled((chest_width / 2) - leg_width / 2, -(chest_height / 2) - leg_height / 2,
                                           leg_width, leg_height, arcade.color.RED)
    shape_list.append(shape)

    # Left arm
    shape = arcade.create_rectangle_filled(-(chest_width / 2) - arm_width / 2 - arm_gap,
                                           (chest_height / 2) - arm_length / 2 - shoulder_height, arm_width, arm_length,
                                           arcade.color.BLUE)
    shape_list.append(shape)

    # Left shoulder
    shape = arcade.create_rectangle_filled(-(chest_width / 2) - (arm_gap + arm_width) / 2,
                                           (chest_height / 2) - shoulder_height / 2, arm_gap + arm_width,
                                           shoulder_height, arcade.color.BLUE_BELL)
    shape_list.append(shape)

    # Right arm
    shape = arcade.create_rectangle_filled((chest_width / 2) + arm_width / 2 + arm_gap,
                                           (chest_height / 2) - arm_length / 2 - shoulder_height, arm_width, arm_length,
                                           arcade.color.BLUE)
    shape_list.append(shape)

    # Right shoulder
    shape = arcade.create_rectangle_filled((chest_width / 2) + (arm_gap + arm_width) / 2,
                                           (chest_height / 2) - shoulder_height / 2, arm_gap + arm_width,
                                           shoulder_height, arcade.color.BLUE_BELL)
    shape_list.append(shape)

    return shape_list


class MyGame(arcade.Window):
    """ Main application class. """

    def __init__(self):
        """ Initializer """
        # Call the parent class initializer
        super().__init__(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE)

        head_radius = 30
        chest_height = 110
        chest_width = 70
        leg_width = 20
        leg_height = 80
        arm_width = 15
        arm_length = 70
        arm_gap = 10
        shoulder_height = 15

        self.draw_counter = 0
        self.shape_list = make_person(head_radius,
                                      chest_height,
                                      chest_width,
                                      leg_width,
                                      leg_height,
                                      arm_width,
                                      arm_length,
                                      arm_gap,
                                      shoulder_height)

        arcade.set_background_color(arcade.color.AMAZON)

        self.ndiImgSender = NDIImageSender.NDIImageSender(b'My_PNG', 10)

    def setup(self):

        """ Set up the game and initialize the variables. """

    def on_draw(self):
        """
        Render the screen.
        """
        
        if self.draw_counter % 10 == 0:
            image_bytes = arcade.get_image().tobytes()
            width = arcade.get_image().width
            height = arcade.get_image().height
            self.ndiImgSender.send_image(image_bytes, width, height)

        # This command has to happen before we start drawing
        arcade.start_render()

        self.shape_list.draw()
        self.draw_counter += 1

    def on_update(self, delta_time):
        """ Movement and game logic """
        if self.shape_list.center_x > SCREEN_WIDTH and self.shape_list.center_y > SCREEN_HEIGHT:
            self.shape_list.center_x = 0
            self.shape_list.center_y = 0

        self.shape_list.center_x += 1
        self.shape_list.center_y += 1
        self.shape_list.angle += .1


def main():
    window = MyGame()
    window.setup()
    arcade.run()


if __name__ == "__main__":
    main()