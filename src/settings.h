/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#pragma once
#include <QCoreApplication>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>

/**
 * @brief Application specific namespace.
 */
namespace hhc
{
/**
 * @brief Stores SmartHHC settings.
 */
class settings
{
public:
	/**
	 * @brief Constructor.
	 */
	settings() {init();}
	/**
	 * @brief Destructor.
	 */
	~settings() {}
	/**
	 * @brief Profile name.
	 */
	QString name() const {return m_name;}
	/**
	 * @brief 1st fixed pedal position value.
	 */
	uint8_t fixed1() const {return m_fixed1;}
	/**
	 * @brief 2nd fixed pedal position value.
	 */
	uint8_t fixed2() const {return m_fixed2;}
	/**
	 * @brief 3rd fixed pedal position value.
	 */
	uint8_t fixed3() const {return m_fixed3;}
	/**
	 * @brief Minimum free pedal position value.
	 */
	uint16_t minPosition() const {return m_min_pos;}
	/**
	 * @brief Maximum free pedal position value.
	 */
	uint16_t maxPosition() const {return m_max_pos;}
	/**
	 * @brief Sets the profile name.
	 */
	void setName(const QString &name) {m_name = name;}
	/**
	 * @brief Sets the 1st fixed pedal position value.
	 */
	void setFixed1(uint8_t value) {m_fixed1 = value;}
	/**
	 * @brief Sets the 2nd fixed pedal position value.
	 */
	void setFixed2(uint8_t value) {m_fixed2 = value;}
	/**
	 * @brief Sets the 3rd fixed pedal position value.
	 */
	void setFixed3(uint8_t value) {m_fixed3 = value;}
	/**
	 * @brief Sets the minimum free pedal position value.
	 */
	void setMinPosition(uint16_t value)  {m_min_pos = value;}
	/**
	 * @brief Sets the maximum free pedal position value.
	 */
	void setMaxPosition(uint16_t value)  {m_max_pos = value;}
	/**
	* @brief Returns the directory path where configuration files are stored.
	*		 e.g. \<$HOME\>/.config/AZDrums/HHC
	*/
	QString path()
	{
		return QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
	}
	/**
	 * @brief Returns the profiles file name extension.
	 */
	QString fileExtension()
	{
		return ".conf";
	}
	/**
	 * @brief Returns the count of the number of profiles stored.
	 */
	int count()
	{
		QDir dir(path());
		dir.setFilter(QDir::Files);
		return dir.count();
	}
	/**
	 * @brief Returns the profile filenames without extension from the config directory as
	 * <a href="http://doc.qt.io/qt-5/qstringlist.html#details">QStringList</a>.
	 */
	QStringList list()
	{
		QDir dir(path());
		QStringList nameList;
		QFileInfoList infoList = dir.entryInfoList(QDir::Files, QDir::Name);
		QListIterator<QFileInfo> it(infoList);
		while(it.hasNext())
		{
			nameList.append(it.next().completeBaseName());
		}
		return nameList;
	}
	void load(const QString &name)
	{
		if (name.isEmpty())
			return;

		QString profileName = name + fileExtension();
		QDir dir(path());
		QString fileName(dir.filePath(profileName));
		QSettings settings(fileName, QSettings::IniFormat);
		settings.beginGroup("profile");
	//  TODO: Check values and types
		m_name	= name;
		m_fixed1  = settings.value("fixed1").toUInt();
		m_fixed2  = settings.value("fixed2").toUInt();
		m_fixed3  = settings.value("fixed3").toUInt();
		m_min_pos = settings.value("min_pos").toUInt();
		m_max_pos = settings.value("max_pos").toUInt();
		m_curve   = settings.value("curve").toStringList();
		settings.endGroup();
	}
	void save(const QString &name = QString())
	{
		QString profileName = name;
		if (name.isEmpty())
			profileName = m_name;

		profileName.append(fileExtension());
		QDir dir(path());
		QString fileName(dir.filePath(profileName));
		QSettings settings(fileName, QSettings::IniFormat);
		settings.beginGroup("profile");
		settings.setValue("fixed1",  m_fixed1);
		settings.setValue("fixed2",  m_fixed2);
		settings.setValue("fixed3",  m_fixed3);
		settings.setValue("min_pos", m_min_pos);
		settings.setValue("max_pos", m_max_pos);
		settings.setValue("curve",   m_curve);
		settings.endGroup();
	}
private:
	/**
	 * @brief Ensures that the profile directory does exists, otherwise creates it.
	 */
	void init()
	{
		QString _path(path());
		QDir dir(_path);
		if (!dir.exists())
			QDir().mkdir(_path);
	}
	QString      m_name	= "Default"; //!< Profile name.
	uint8_t      m_fixed1  = 10,     //!< 1st fixed pedal position value.
							 m_fixed2  = 20,     //!< 2nd fixed pedal position value.
							 m_fixed3  = 30;     //!< 3rd fixed pedal position value.
	uint16_t     m_min_pos = 600,    //!< Minimum free pedal position value.
							 m_max_pos = 900;    //!< Maximum free pedal position value.
	QStringList  m_curve = {"0","0","0","0","1","1","1","1","1","2","2","2","2","2","3","3",
							 "3","3","3","4","8","4","4","5","5","5","6","6","6","6","6","6",
							 "6","7","7","7","7","7","7","7","7","7","8","8","8","8","8","8",
							 "8","8","8","8","8","9","9","9","9","9","10","10","10","11","11","11",
							 "12","12","12","13","13","13","14","14","15","15","16","16","17","17","18","18",
							 "19","20","20","21","21","22","22","23","23","24","24","25","25","26","26","27",
							 "27","28","28","29","29","30","33","37","40","44","47","50","54","57","61","64",
							 "67","71","74","78","81","84","88","91","95","98","101","105","108","112","114","115"};
};
} // namespace hhc
