/**
  @file accesxml.h
  @brief implémentation de la classe accesxml
  @version 1.0
  @author LEVILLAIN Dimitri
  @date 23/04/2019
  @details Classe gérant le fichier de configuration XML
  */

#include "accesxml.h"
/**
 * @brief AccesXML::AccesXML
 * @details Constructeur
 */
AccesXML::AccesXML()
{
    configurationBDD = NULL;
}
/**
 * @brief AccesXML::LireConfiguration
 * @param fichierConfig
 */
void AccesXML::LireConfiguration(QString fichierConfig)
{
    QFile fichier(fichierConfig);
    if(!fichier.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Impossible de lire le fichier : " << fichier.errorString();
    }
    else
    {
        QXmlStreamReader lecteurXML(&fichier);

        if (lecteurXML.readNextStartElement()) {
            if (lecteurXML.name() == "configuration")
            {
                configurationBDD = new typeConfigurationBDD;    //Création de la configuration
                while(lecteurXML.readNextStartElement())
                {
                    if (lecteurXML.name() == "ipServeurBDD")
                    {
                        configurationBDD->ipServeurBDD = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "portServeurBDD")
                    {
                        configurationBDD->portServeurBDD = lecteurXML.readElementText().toInt();
                    }
                    if (lecteurXML.name() == "nomBDD")
                    {
                        configurationBDD->nomBDD = lecteurXML.readElementText();
                    }                    
                    if (lecteurXML.name() == "utilisateurBDD")
                    {
                        configurationBDD->utilisateurBDD = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "motDePasseBDD")
                    {
                        configurationBDD->motDePasseBDD = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "ipServeurBDDAbonnes")
                    {
                        configurationBDD->ipServeurBDDAbonnes = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "portServeurBDDAbonnes")
                    {
                        configurationBDD->portServeurBDDAbonnes = lecteurXML.readElementText().toInt();
                    }
                    if (lecteurXML.name() == "nomBDDAbonnes")
                    {
                        configurationBDD->nomBDDAbonnes = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "utilisateurBDDAbonnes")
                    {
                        configurationBDD->utilisateurBDDAbonnes = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "motDePasseBDDAbonnes")
                    {
                        configurationBDD->motDePasseBDDAbonnes = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "cheminImageClient")
                    {
                        cheminImageClient = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "cheminDossierImages")
                    {
                        cheminDossierImages = lecteurXML.readElementText();
                    }
                    if (lecteurXML.name() == "telephoneDefaut")
                    {
                        telephoneDefaut = lecteurXML.readElementText();
                    }
                }
            }
            else
            {
                lecteurXML.raiseError(QObject::tr("Fichier incorrect."));
            }
        }
        fichier.close();        //Fermeture du fichier
    }
}
/**
 * @brief AccesXML::ObtenirConfigurationBDD
 * @return
 */
typeConfigurationBDD *AccesXML::ObtenirConfigurationBDD() const
{
    return configurationBDD;
}
