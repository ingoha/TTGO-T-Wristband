#ifndef ABSTRACTPAGE_H
#define ABSTRACTPAGE_H
class AbstractPage {
  public:
    //
    // draw the page's UI
    //
    virtual void draw(bool initialLoad);
    //
    // If button is (long) pressed on the page
    //
    virtual void action();
};
#endif