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

#ifndef KNMUSICLIBRARYARTISTTAB_H
#define KNMUSICLIBRARYARTISTTAB_H

#include <QModelIndex>

#include "knmusiclibrarycategorytab.h"

class KNDropProxyContainer;
class KNCategoryTab;
class KNMusicCategoryDisplay;
class KNMusicCategoryListViewBase;
class KNMusicLibraryArtistTab : public KNMusicLibraryCategoryTab
{
    Q_OBJECT
public:
    explicit KNMusicLibraryArtistTab(QWidget *parent = 0);

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showInAction().
     */
    QAction *showInAction() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::tab().
     */
    QAbstractButton *tab() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showInTab().
     */
    void showInTab(const KNMusicDetailInfo &detailInfo) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showIndex().
     */
    void showIndex(KNMusicModel *musicModel,
                   const QModelIndex &index) Q_DECL_OVERRIDE;

signals:

public slots:
    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::setCategoryModel().
     */
    void setCategoryModel(KNMusicCategoryModel *model) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::setLibraryModel().
     */
    void setLibraryModel(KNMusicLibraryModel *model) Q_DECL_OVERRIDE;

protected:
    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::showEvent().
     */
    void showEvent(QShowEvent *event);

    /*!
     * \brief Reimplemented from KNMusicLibraryCategoryTab::hideEvent().
     */
    void hideEvent(QHideEvent *event);

private slots:
    void retranslate();
    void checkCategorySelected();
    void onActionShowInArtist();
    void onActionCategoryIndexChanged(const QModelIndex &index);

private:
    inline void showAndSelectRow(const int &musicRow);
    QMetaObject::Connection m_notEmptyCheck;
    QModelIndex m_currentSourceIndex;
    KNCategoryTab *m_tab;
    QAction *m_showInArtistTab;
    KNDropProxyContainer *m_dropProxy;
    KNMusicCategoryListViewBase *m_artistList;
    KNMusicCategoryDisplay *m_artistDisplay;
    KNMusicCategoryModel *m_categoryModel;
    KNMusicLibraryModel *m_libraryModel;
};

#endif // KNMUSICLIBRARYARTISTTAB_H