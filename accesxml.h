#ifndef ACCESXML_H
#define ACCESXML_H

#include <QXmlStreamReader>
#include <QDebug>
#include <QFile>

//Structure des information de la base de données
struct typeConfigurationBDD
{
    QString ipServeurBDD;
    int portServeurBDD;
    QString nomBDD;
    QString utilisateurBDD;
    QString motDePasseBDD;

    QString ipServeurBDDAbonnes;
    int portServeurBDDAbonnes;
    QString nomBDDAbonnes;
    QString utilisateurBDDAbonnes;
    QString motDePasseBDDAbonnes;
};

class AccesXML
{
public:
    AccesXML();

    void LireConfiguration(QString fichierConfig);
    typeConfigurationBDD *ObtenirConfigurationBDD() const;

private:
    //Attributs
    typeConfigurationBDD *configurationBDD;     //Les configurations de la base de données.
    QString cheminDossierImages;                //Chemin du dossier des image main courante.
    QString cheminImageClient;                  //Chemin du logo du client.
    QString telephoneDefaut;                    //Identifiant du téléphone de scanner par défaut.
};

#endif // ACCESXML_H
