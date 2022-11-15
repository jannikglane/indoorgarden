#ifndef MENU_H
#define MENU_H

enum menuType {
    base,
    lightControl,
    waterControl,
    tempControl
};

class Menu
{
private:
    int menuPos = 0;
    MenuItem entries[];
public:
    Menu();
    ~Menu();
};

class MenuItem
{
private:
    String title;
public:
    MenuItem();
    ~MenuItem();
};

MenuItem::MenuItem()
{
}

MenuItem::~MenuItem()
{
}


#endif