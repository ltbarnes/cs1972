#ifndef MANAGER_H
#define MANAGER_H

class World;
class Graphics;
enum GraphicsMode;

class Manager
{
public:
    Manager() { m_drawable = false; }
    Manager(GraphicsMode gm) { m_gm = gm; m_drawable = true; }
    virtual ~Manager() {}

    bool isDrawable() { return m_drawable; }
    GraphicsMode getGraphicsMode() { return m_gm; }

    virtual void manage(World *world, float onTickSecs) = 0;
    virtual void onDraw(Graphics *) {}

private:
    GraphicsMode m_gm;
    bool m_drawable;

};

#endif // MANAGER_H
