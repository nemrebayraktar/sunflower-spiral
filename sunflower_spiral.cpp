// SUNFLOWER SPIRAL
// Advanced Computer Graphics Project
// Student:Necdet Emre Bayraktar
// Standart Includes
#include <Gorgon/Geometry/Point.h>
#include <Gorgon/Graphics/Bitmap.h>
#include <Gorgon/Graphics/Color.h>
#include <Gorgon/Main.h>
#include <Gorgon/Time.h>
#include <Gorgon/UI.h>
#include <Gorgon/UI/Dialog.h>
#include <Gorgon/UI/Organizers/Flow.h>
#include <Gorgon/UI/Window.h>
#include <Gorgon/Widgets/Button.h>
#include <Gorgon/Widgets/GeometryBoxes.h>
#include <Gorgon/Widgets/Inputbox.h>
#include <Gorgon/Widgets/Layerbox.h>
#include <Gorgon/Widgets/Numberbox.h>
#include <Gorgon/Widgets/Panel.h>
#include <array>
#include <cmath>
#include <string>
// PI constant definition
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// standart namespaces
namespace Color = Gorgon::Graphics::Color;
namespace UI = Gorgon::UI;
namespace Widgets = Gorgon::Widgets;
using namespace Gorgon::UI::literals;
// Template's data structures
struct AnimationData {
Gorgon::Geometry::Pointf location;
int size;
float speed = 0.f;
};
// Gorgon template classes
class Application {
public:
Application(UI::Window &window);
void Run();
void DoFrame(unsigned int delta);
bool Quit();
void Redraw();
private:
void buildsettingsui();
// UI widgets
Widgets::Pointfbox start;
Widgets::Integerbox size;
Widgets::Panel side;
UI::Organizers::Flow sideorganizer;
Widgets::Layerbox layerbox;
Widgets::Button run, save, zoomin, zoomout;
// Graphics
Gorgon::Graphics::Layer targetlayer;
Gorgon::Graphics::Bitmap bitmap, zoomed;
// Zoom settings
static constexpr std::array<int, 8> zoomrates = {1, 2, 3, 4, 6, 8, 12, 16};
int zoom = 0;
UI::Window &window;
AnimationData data;
};
// mySpeed variable
static float mySpeed = 0.0f;
// Template's UI
void Application::buildsettingsui() {
start = {20.f, 30.f};
size = 10;
sideorganizer << 4 << "Start" << start << sideorganizer.Break
<< 4 << "Size" << size << sideorganizer.Break;
}
// template's run function
void Application::Run() {
bitmap.Resize(600, 600, Gorgon::Graphics::ColorMode::RGBA);
data = AnimationData{start, size, 0.f};
mySpeed = 0.0f;
}
//DoFrame algorithm
void Application::DoFrame(unsigned int delta) {
if(!bitmap.HasData()) return;
bitmap.Clear();
// for spirals infinite loop
mySpeed +=0.05f;
//for spirals location(I changed the size with 600,600 so it will begin 300,300)
float centerX =bitmap.GetSize().Width / 2.0f;
float centerY =bitmap.GetSize().Height / 2.0f;
// parameters for spiral (I decided to 7 because of the medium density)
float scale =7.0f;
//golden angle rule =137.508*(PI/180)
double goldenAngle =137.508 * (M_PI / 180.0);
//we can change the speed with this multiplication rate
float rotationOffset =mySpeed *10.5f;
//for seed number
const int seedNumber =1500;
// for gradient setup
float maxRadius =scale *sqrt(seedNumber);
// this is for main loop
for(int n= 0; n < seedNumber; ++n) {
//we can set the angles with golden angle
double angle =(n * goldenAngle) + rotationOffset;
// I decide to use Archimedean spiral for equal spaces
float r =scale * sqrt(n);
// classic polar to cartesian rotations
int x =centerX + r * cos(angle);
int y =centerY + r * sin(angle);
// radial gradient
// center to outside of spiral
// Normalized distance: d =r / maxRadius (0.0 - 1.0)
float normalizedDistance =r / maxRadius;
auto seedcolor =Color::Yellow;
if (normalizedDistance < 0.25f) {
// 0-25 Center zone
seedcolor =Color::Brown;
 } else if (normalizedDistance < 0.5f) {
// 25-50 second zone
seedcolor =Color::Orange;
} else if (normalizedDistance < 0.75f) {
// 50-75 third zone
seedcolor =Color::Gold;
} else {
// 75-100 fourth zone
seedcolor =Color::Yellow;
}
// for real sunflower experience seeds will grow with this algorithm
float normalizedIndex = static_cast<float>(n) / seedNumber;
int seedSize =2 + static_cast<int>(normalizedIndex * 3);
// program didn’t work without this security check and I added this.
if (x > seedSize && x < bitmap.GetSize().Width - seedSize &&
y > seedSize && y < bitmap.GetSize().Height - seedSize) {
// dy draws y pixels and dx draws x pixels
for(int dy =0; dy <=seedSize; dy++) {
for(int dx =0; dx <=seedSize; dx++) {
bitmap.SetRGBAAt(x + dx, y + dy, seedcolor);
}
}
}
}
//and finally program will draw our sunflower spiral
Redraw();
}
//Application part
// Template's UI
Application::Application(UI::Window& window) : window(window) {
window.Add(side);
side.SetInteriorWidth(8_u);
side.SetHeight(100_perc);
side.AttachOrganizer(sideorganizer);
window.AddNextTo(layerbox);
layerbox.SetWidth(window.GetInteriorSize().Width - layerbox.GetCurrentLocation().X);
layerbox.SetHeight(100_perc);
window.ResizedEvent.Register([this] {
layerbox.SetWidth(this->window.GetInteriorSize().Width -
layerbox.GetCurrentLocation().X);
});
layerbox.GetLayer().Add(targetlayer);
run.Text = "Run";
window.SetDefault(run);
run.SetWidth(4);
run.ClickEvent.Register(*this, &Application::Run);
save.Text = "Save";
save.SetWidth(4);
save.ClickEvent.Register([this] {
if(!bitmap.HasData()) {
UI::ShowMessage("Save", "Nothing to save, click run first");
return;
}
UI::Input<std::string>("Save", "Enter filename", [this](std::string fname) {
if(fname.find_first_of('.') == fname.npos) fname += ".png";
if(!bitmap.Export(fname)) UI::ShowMessage("Save", "Save failed");
});
});
zoomin.Text = "Zoom in";
zoomin.SetWidth(4);
zoomin.ClickEvent.Register([this] {
if(zoom < zoomrates.size() - 1) {
zoom++;
Redraw();
}
});
zoomout.Text = "Zoom out";
zoomout.SetWidth(4);
zoomout.ClickEvent.Register([this] {
if(zoom > 0) {
zoom--;
Redraw();
}
});
sideorganizer << run << save << zoomin << zoomout;
buildsettingsui();
Gorgon::BeforeFrameEvent.Register([this]() {
this->DoFrame(Gorgon::Time::DeltaTime());
});
}
// Gorgon's quit function
bool Application::Quit() {
return true;
}
// template's redraw function
void Application::Redraw() {
targetlayer.Clear();
if(bitmap.HasData()) {
auto rate = zoomrates[zoom];
zoomed = bitmap.ZoomMultiple(rate);
if(zoom > 4) {
zoomed.ForAllPixels([rate, this](int x, int y) {
if(x % rate == 0 || y % rate == 0)
zoomed(x, y, 0) = 64 + zoomed(x, y, 0) / 2;
});
}
zoomed.Prepare();
zoomed.Draw(targetlayer, 0, 0);
}
}
// Static member initialization code
constexpr std::array<int, 8> Application::zoomrates;
// Template's main function
int main() {
Gorgon::Initialize("CG");
Gorgon::UI::Window window({1200, 900}, "Sunflower Spiral");
Gorgon::UI::Initialize();
Application app(window);
window.AllowResize();
window.ClosingEvent.Register([&app](bool &allow) {
allow = app.Quit();
});
window.Run();
return 0;
}
