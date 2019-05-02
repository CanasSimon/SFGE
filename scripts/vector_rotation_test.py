from SFGE import *


class VectorRotationTest(System):
    time = 0.0
    rotation_speed = 0.0
    u = p2Vec2()
    n = p2Vec2()
    r = p2Vec2()

    pu = Vec2f()
    pn = Vec2f()
    pr = Vec2f()

    def init(self):
        self.time = 0.0
        self.rotation_speed = .9

        self.u = p2Vec2(.08, -0.02)
        self.n = p2Vec2(0, -.1)

    def update(self, dt):
        self.time += dt
#        self.u = self.u.rotate(self.rotation_speed * self.time)
#        self.n = self.u.get_normal()
        self.r = self.u.get_reflection(self.n)

        self.pu = Physics2dManager.meter2pixel(self.u)
        self.pn = Physics2dManager.meter2pixel(self.n)
        self.pr = Physics2dManager.meter2pixel(self.r)

    def on_draw(self):
        graphics2d_manager.draw_vector(self.pu, Vec2f(600, 400), Color.Red)
        graphics2d_manager.draw_vector(self.pn, Vec2f(600, 400), Color.Green)
        graphics2d_manager.draw_vector(self.pr, Vec2f(600, 400), Color.Magenta)
