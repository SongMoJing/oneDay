#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QIcon>
#include <QTimer>
#include <QPainter>
#include <QBrush>
#include <QScreen>
#include <QProgressBar>
#include <QPainterPath>
#include <QFile>

const char *const path_icon = "res/app icon.png";
const char *const path_config = "res/config.ini";

class MainWindow : public QWidget {
public:
    MainWindow() {
        // 设置无边框，透明背景，并保持在最上层
        setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
        setAttribute(Qt::WA_TranslucentBackground);  // 透明背景
        setAttribute(Qt::WA_OpaquePaintEvent);  // 防止背景透明时出现渲染问题

        // 获取屏幕的大小（Qt6 使用 QScreen）
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->availableGeometry();  // 获取可用的屏幕区域
        int screenWidth = screenGeometry.width();

        // 设置窗口大小
        setFixedSize((screenWidth / 5) * 3, 65);

        // 计算窗口的位置，使其在屏幕上方正中央
        int x = (screenWidth - width()) / 2;
        move(x, 5);

        ///////////////////////////////////////////////////////////////

        // 设置字体
        font = new QFont("Microsoft YaHei", 12, QFont::Bold);

        // 创建进度条
        progressBar = new QProgressBar(this);
        progressBar->setGeometry(0, 60, width(), 5);  // 设置进度条的位置和大小
        // TODO
        progressBar->setRange(0, 500);  // 设置进度条的范围
        progressBar->setValue(0);  // 初始化进度条的值
        progressBar->setTextVisible(false);  // 不显示进度条的文本
        // 设置进度条背景颜色为渐变
        progressBar->setStyleSheet(R"(
            QProgressBar {
                border-radius: 5px;
                background: #444;
            }
            QProgressBar::chunk {
                border-radius: 5px;
                background: #777;
            }
        )");

        // 创建一个定时器来模拟进度条的更新
        progressTimer = new QTimer(this);
        connect(progressTimer, &QTimer::timeout, this, &MainWindow::updateProgress);
        // TODO
        progressTimer->start(1000);  // 每100ms更新一次进度条

        ///////////////////////////////////////////////////////////////

        // 显示窗口
        show();

        // 连接系统托盘图标的显示/隐藏功能
        trayIcon = new QSystemTrayIcon(QIcon(path_icon), this);

        auto *trayMenu = new QMenu(this);
        auto *toggleAction = new QAction("显示/隐藏窗口", this);
        connect(toggleAction, &QAction::triggered, this, &MainWindow::toggleWindowVisibility);
        auto *exitAction = new QAction("退出软件", this);
        connect(exitAction, &QAction::triggered, &QApplication::quit);

        trayMenu->addAction(toggleAction);
        trayMenu->addAction(exitAction);
        trayIcon->setContextMenu(trayMenu);

        // 显示托盘图标
        trayIcon->show();
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        // 确保绘制整个窗口
        QPainter painter(this);
        QBrush brush(QColor(0, 0, 0, 80));  // 黑色半透明背景
        painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
        // 创建带圆角的矩形路径
        QPainterPath panPath;
        int radius = 5;  // 圆角半径
        panPath.addRoundedRect(0, 0, width(), height(), radius, radius);

        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);  // 防止绘制边框
        // 绘制圆角矩形
        painter.fillPath(panPath, brush);

        // 绘制文字
        font->setPointSize(18);  // 设置字体大小
        font->setBold(false);  // 设置字体加粗
        painter.setFont(*font);
        painter.setPen(QColor(255, 255, 255));  // 设置文字颜色为白色

        // TODO
        QString text = "One Day";
        QRect textRect = QRect(0, 0, width() - 30, 35);
        painter.drawText(textRect, Qt::AlignCenter, text);  // 将文字绘制在窗口上方正中
    }

private:
    QSystemTrayIcon *trayIcon; // 系统托盘图标
    QProgressBar *progressBar; // 进度条
    QTimer *progressTimer;     // 定时器
    QFont *font; // 字体

    void toggleWindowVisibility() {
        if (isVisible()) {
            hide();
        } else {
            show();
        }
    }

    void updateProgress() {
        // 更新进度条的值
        int value = progressBar->value();
        if (value < progressBar->maximum()) {
            progressBar->setValue(value + 1);  // 模拟进度条增长
        } else {
            progressTimer->stop();  // 停止定时器
            // TODO
            trayIcon->showMessage("结束", "倒计时已结束！", QSystemTrayIcon::Information, 3000);
        }
    }
};

int main(int argc, char *argv[]) {

    QApplication application(argc, argv);

    // 读取文件
    QFile file("C:/Users/86183/Desktop/One Day.txt");

    // 创建主窗口对象
    MainWindow window;

    return QApplication::exec();
}
