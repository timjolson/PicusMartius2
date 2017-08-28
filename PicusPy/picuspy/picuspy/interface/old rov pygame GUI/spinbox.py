import pygame
class spinBox:
    font = pygame.font.Font(None, 18)

    def __init__(self,step):
        self.state = 0
        self.step = step
        #TODO: load value

    def make(self, position):
        self.size = (58, 30)
        self.rect = pygame.Rect(position, self.size)
        self.image = pygame.Surface(self.rect.size)
        self.image.fill((55, 155, 255))

        self.buttonRects = [pygame.Rect(self.size[0] - 15, 0, 20, self.size[1] / 2),
                            pygame.Rect(self.size[0] - 15, self.size[1] / 2, 20, self.size[1] / 2)]

    def draw(self, surface):
        #Draw SpinBox onto surface
        textline = spinBox.font.render(str(self.state), True, (255,255,255))

        self.image.fill((55,155,255))

        #increment button
        pygame.draw.rect(self.image, (255,255,255), self.buttonRects[0])
        pygame.draw.polygon(self.image, (55,155,255), [(self.size[0]-12,12), (self.size[0]-7,2), (self.size[0]-2,12)])
        #decrement button
        pygame.draw.rect(self.image, (255,255,255), self.buttonRects[1])
        pygame.draw.polygon(self.image, (55,155,255), [(self.size[0]-12,18), (self.size[0]-7,28), (self.size[0]-2,18)])

        self.image.blit(textline, (5, (self.rect.height - textline.get_height()) // 2))

        surface.blit(self.image, self.rect)

    def increment(self):
        self.state += self.step

    def decrement(self):
        self.state -= self.step

    def __call__(self, position):
        #enumerate through all button rects
        for idx, btnR in enumerate(self.buttonRects):
            #create a new pygame rect with absolute screen position
            btnRect = pygame.Rect((btnR.topleft[0] + self.rect.topleft[0],
                                   btnR.topleft[1] + self.rect.topleft[1]), btnR.size)

            if btnRect.collidepoint(position):
                if idx == 0:
                    self.increment()
                else:
                    self.decrement()

    def getState(self):
        return self.state

    def setState(self, newstate):
        self.state = newstate
        return self.state

    buttonRects = [pygame.Rect(0,0,0,0),pygame.Rect(0,0,0,0)]
    rect = pygame.Rect((0,0), (0,0))
    size = [0,0]