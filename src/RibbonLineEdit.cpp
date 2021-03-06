/*
 * Copyright (C) 2020 Adrian Carpenter
 *
 * This file is part of the Nedrysoft Ribbon Bar. (https://github.com/nedrysoft/qt-ribbon)
 *
 * A cross-platform ribbon bar for Qt applications.
 *
 * Created by Adrian Carpenter on 02/12/2020.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "RibbonLineEdit.h"

#include <QApplication>
#include <QMimeData>
#include <QPainter>
#include <QPicture>
#include <QResizeEvent>
#include <QSpacerItem>

constexpr auto ThemeStylesheet = R"(
    QTextEdit {
        [background-colour];
        height: 13px;
        [border];
        padding: 0px;
    }

    QTextEdit:focus {
        [border];
    }
)";

constexpr auto WidgetHeight = 21;

Nedrysoft::Ribbon::RibbonLineEdit::RibbonLineEdit(QWidget *parent) :
        QTextEdit(parent) {

    setAttribute(Qt::WA_MacShowFocusRect,false);

    auto themeSupport = Nedrysoft::ThemeSupport::ThemeSupport::getInstance();

    connect(themeSupport, &Nedrysoft::ThemeSupport::ThemeSupport::themeChanged, [=](bool isDarkMode) {
        updateStyleSheet(isDarkMode);
    });

    connect(this, &Nedrysoft::Ribbon::RibbonLineEdit::textChanged, [=]() {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma ide diagnostic ignored "VirtualCallInCtorOrDtor"
#endif
        resizeEvent(nullptr);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
    });

    updateStyleSheet(themeSupport->isDarkMode());

    setLineWrapMode(QTextEdit::NoWrap);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    if (placeholderText().isEmpty()) {
        setPlaceholderText(" ");
    }

    QTextEdit::setMinimumHeight(WidgetHeight);
    QTextEdit::setMaximumHeight(WidgetHeight);

    QTextEdit::setSizePolicy(sizePolicy().horizontalPolicy(), QSizePolicy::Fixed);

#if (QT_VERSION >= QT_VERSION_CHECK(5, 12,0))
    QPalette palette = QTextEdit::palette();

    if (themeSupport->isDarkMode()) {
        palette.setColor(QPalette::PlaceholderText, QColor(Qt::darkGray).lighter(125));
    } else {
        palette.setColor(QPalette::PlaceholderText, Qt::darkGray);
    }

    setPalette(palette);
#endif
}

void Nedrysoft::Ribbon::RibbonLineEdit::setMinimumHeight(int minimumHeight) {
    Q_UNUSED(minimumHeight)

    QTextEdit::setMinimumHeight(WidgetHeight);
}

void Nedrysoft::Ribbon::RibbonLineEdit::setMaximumHeight(int maximumHeight) {
    Q_UNUSED(maximumHeight)

    QTextEdit::setMaximumHeight(WidgetHeight);
}

void Nedrysoft::Ribbon::RibbonLineEdit::setSizePolicy(QSizePolicy policy) {
    QTextEdit::setSizePolicy(policy.horizontalPolicy(), QSizePolicy::Fixed);
}

void Nedrysoft::Ribbon::RibbonLineEdit::setSizePolicy(QSizePolicy::Policy horizontal, QSizePolicy::Policy vertical) {
    Q_UNUSED(vertical)

    QTextEdit::setSizePolicy(horizontal, QSizePolicy::Fixed);
}

auto Nedrysoft::Ribbon::RibbonLineEdit::event(QEvent *e) -> bool {
    switch(e->type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease: {
            auto keyEvent = dynamic_cast<QKeyEvent *>(e);

            switch(keyEvent->key()) {
                case Qt::Key_Enter:
                case Qt::Key_Return: {
                    e->accept();

                    return true;
                }

                default: {
                    break;
                }
            }

            break;
        }

        default: {
            break;
        }
    }

    return QTextEdit::event(e);
}

Nedrysoft::Ribbon::RibbonLineEdit::~RibbonLineEdit() {
}

auto Nedrysoft::Ribbon::RibbonLineEdit::updateStyleSheet(bool isDarkMode) -> void {
    QString styleSheet(ThemeStylesheet);

    styleSheet.replace("[border]", "border: 0px none");

    if (isDarkMode) {
        styleSheet.replace("[background-colour]", "background-color: #434343");
    } else {
        styleSheet.replace("[background-colour]", "background-color: #ffffff");
    }

    setStyleSheet(styleSheet);
}

auto Nedrysoft::Ribbon::RibbonLineEdit::resizeEvent(QResizeEvent *event) -> void {
    QFontMetrics fontMetrics(font());

    auto text = toPlainText().isEmpty()?placeholderText():toPlainText();
    auto offset = (rect().height()-fontMetrics.boundingRect(text).height())/2;

    if (offset!=document()->documentMargin()) {
        document()->setDocumentMargin(offset);
    }
}

void Nedrysoft::Ribbon::RibbonLineEdit::insertFromMimeData(const QMimeData *source) {
    if (source->hasText() || source->hasHtml()) {
        auto plainString = source->text();

        plainString = plainString.replace("\r", "");
        plainString = plainString.replace("\n", "");

        this->insertPlainText(plainString);
    }
}
