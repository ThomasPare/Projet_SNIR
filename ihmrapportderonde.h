#ifndef IHMRAPPORTDERONDE_H
#define IHMRAPPORTDERONDE_H

#include <QWidget>
#include <QDebug>
#include <QFileInfo>
#include <QIcon>
#include <QCheckBox>
#include <QPushButton>
#include <QProcess>

#include "accesxml.h"
#include "accesbdd.h"

namespace Ui {
class IHMRapportDeRonde;
}

class IHMRapportDeRonde : public QWidget
{
    Q_OBJECT

public:
    explicit IHMRapportDeRonde(QWidget *parent = 0);
    ~IHMRapportDeRonde();

private slots:
    void SetThemes();

    void on_pushButtonDate_clicked();

    void on_pushButtonDateFin_clicked();

    void on_calendarWidget_clicked(const QDate &date);

    void on_calendarWidgetFin_clicked(const QDate &date);

    void AjoutNomColonne();

    void on_lineEditAgent_textChanged(const QString &arg1);

    void on_lineEditRonde_textChanged(const QString &arg1);

    void AfficherRondeDansTableau();

    void RemplirListe();

    void on_tableWidgetRonde_cellClicked(int row);

    void AfficherPointeauxDansTableau();

    void on_tableWidgetPointeaux_cellClicked(int row);

    void OnQPushButtonPhoto_clicked();

    void on_pushButtonFermerPhoto_clicked();

    void on_pushButtonApercuPDF_clicked();

    void on_pushButtonRetour_clicked();

    void on_pushButtonValiderListe_clicked();

    void on_pushButtonSupprimerAbonne_clicked();



    void on_pushButtonRafraichirInterface_clicked();

    void RemplirListeRonde();

    void RemplirListeAbonnes();

    void on_tabWidget_currentChanged(int index);

    void on_comboBoxListeAbonnees_activated(const QString &arg1);

private:
    Ui::IHMRapportDeRonde *ui;

    QStringList nomColonneRonde = {"Ronde","Agent","Date"};
    QStringList nomColonnePointeaux = {"Numéro pointeaux","Retard","Heure de pointage","Commentaire","Photo"};

    QList<QString> listeNomAgents;
    QList<QString> listeNomRondes;
    QList<QDateTime> listeDate;
    QList<QDateTime> listeDatePointeau;
    QList<int> listeIdRonde;
    QList<int> listeIdPointeaux;
    QList<int> listeIdAgent;

    int ligneRonde = 0;
    int lignePointeaux = 0;

    AccesXML *xmlConfig;
    const QString CHEMINXML = "/home/USERS/ELEVES/SNIR2017/tpare/SNIR2/Projet/rapportderonde/configurations.xml";

    AccesBdd *mySqlBdd;
    AccesBdd *BddAbonnes;

    int numeroImage;
    QList<QPushButton*> listeBoutonImage;

    typeConfigurationBDD *configBDD = new typeConfigurationBDD;

    int nombreAbonnesSelectionne;
};

#endif // IHMRAPPORTDERONDE_H
