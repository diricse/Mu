/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICCATEGORYDISPLAY_H
#define KNMUSICCATEGORYDISPLAY_H

#include <QWidget>

class QGraphicsOpacityEffect;
class QLabel;
class KNScrollLabel;
class KNSideShadowWidget;
class KNMusicTab;
class KNMusicProxyModel;
class KNMusicLibraryTreeView;
class KNMusicLibraryModel;
/*!
 * \brief The KNMusicCategoryDisplay class provides a widget which could display
 * the detail of one category information of the song library.
 */
class KNMusicCategoryDisplay : public QWidget
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicCategoryDisplay widget.
     * \param parent The parent widget.
     * \param tab The music tab of the tree view.
     */
    explicit KNMusicCategoryDisplay(QWidget *parent = 0, KNMusicTab *tab = 0);

    /*!
     * \brief Get the proxy music model of the category.
     * \return The proxy model pointer.
     */
    KNMusicProxyModel *proxyMusicModel() const;

signals:
    /*!
     * \brief When the tree view is dragging song row, this signal should be
     * emitted to show the playlist list widget.
     */
    void requireShowPlaylistList();

    /*!
     * \brief When the tree view stops dragging song row, this signal should be
     * emitted to hide the playlist list widget.
     */
    void requireHidePlaylistList();

public slots:
    /*!
     * \brief Set the library model to the detail tree view.
     * \param model The library model pointer.
     */
    void setLibraryModel(KNMusicLibraryModel *model);

    /*!
     * \brief Scroll to specific row by given the row of song in the music
     * library model.
     * \param row The row of the song in music library model.
     */
    void scrollToSourceRow(int row);

    /*!
     * \brief Show the no category item.
     * \param title The title text of the no category item.
     */
    void showNoCategoryItem(const QString &title);

    /*!
     * \brief Set the category filter column.
     * \param column The column index.
     */
    void setCategoryColumn(const int &column);

    /*!
     * \brief Set the category fitler text, after setting the category filter
     * and the viewer will display items belong to this category.
     * \param text The category content text.
     */
    void setCategoryText(const QString &text);

    /*!
     * \brief Set the category display pixmap icon.
     * \param pixmap The category album art.
     */
    void setCategoryIcon(const QPixmap &pixmap);

    /*!
     * \brief When the viewer need to hide all stuffs, like no search result, we
     * have to hide all the widgets.
     */
    void hideAllStuffs();

    /*!
     * \brief Set the identifier of the proxy model.
     * \param identifier The proxy model identifier text.
     */
    void setProxyModelIdentifier(const QString &identifier);

protected:
    /*!
     * \brief Reimplemented from QWidget::resizeEvent().
     */
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from QWidget::paintEvent().
     */
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private slots:
    void retranslate();
    void onActionSearch();

private:
    inline void showAllStaffs();
    inline void updateTitle();
    inline void updateDetailInfo();
    inline void updateBackgroundIcon();
    inline void updateBackgroundIconContent();
    QRadialGradient m_iconGradient;
    QPixmap m_categoryIcon, m_backgroundImage;
    QString m_songCount[3], m_searchCount[3], m_searchResultIn, m_titleText;
    KNScrollLabel *m_categoryTitle;
    QLabel *m_categoryInfo;
    KNMusicLibraryTreeView *m_categoryTreeView;
    KNSideShadowWidget *m_leftShadow;
    int m_backgroundSize;
};

#endif // KNMUSICCATEGORYDISPLAY_H
