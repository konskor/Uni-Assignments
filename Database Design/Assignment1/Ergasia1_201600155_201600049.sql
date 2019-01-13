-- MySQL Script generated by MySQL Workbench
-- Thu Apr  5 20:45:48 2018
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema Car_Accidents_DB
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema Car_Accidents_DB
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `Car_Accidents_DB` DEFAULT CHARACTER SET utf8 ;
USE `Car_Accidents_DB` ;

-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Traffic Department`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Traffic Department` (
  `ID` VARCHAR(45) NOT NULL,
  `Name` VARCHAR(45) NOT NULL,
  `Phone_Number` VARCHAR(45) NOT NULL,
  `Address` VARCHAR(45) NOT NULL,
  `Postal_Code` VARCHAR(45) NOT NULL,
  `City` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idTraffic Police_UNIQUE` (`ID` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Police officer`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Police officer` (
  `ID_Badge` VARCHAR(45) NOT NULL COMMENT 'Unique key, because we avoid having',
  `Name` VARCHAR(45) NOT NULL,
  `Rank` VARCHAR(45) NOT NULL,
  `Traffic Department_ID1` VARCHAR(45) NOT NULL,
  `Police officer_ID_Badge` VARCHAR(45) NULL,
  PRIMARY KEY (`ID_Badge`),
  UNIQUE INDEX `ID_Badge_UNIQUE` (`ID_Badge` ASC),
  INDEX `fk_Police officer_Traffic Department1_idx` (`Traffic Department_ID1` ASC),
  INDEX `fk_Police officer_Police officer1_idx` (`Police officer_ID_Badge` ASC),
  CONSTRAINT `fk_Police officer_Traffic Department1`
    FOREIGN KEY (`Traffic Department_ID1`)
    REFERENCES `Car_Accidents_DB`.`Traffic Department` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Police officer_Police officer1`
    FOREIGN KEY (`Police officer_ID_Badge`)
    REFERENCES `Car_Accidents_DB`.`Police officer` (`ID_Badge`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`DOTA`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`DOTA` (
  `Protocol_Number` INT NOT NULL,
  PRIMARY KEY (`Protocol_Number`),
  UNIQUE INDEX `idDOTA_UNIQUE` (`Protocol_Number` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Street_Details_DB`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Street_Details_DB` (
  `ID` INT NOT NULL,
  `Type` VARCHAR(45) NOT NULL,
  `Condition` VARCHAR(45) NOT NULL,
  `Speed_Limit` INT NOT NULL,
  PRIMARY KEY (`ID`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Report`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Report` (
  `DOTA` INT NOT NULL,
  `Report_Date` DATE NOT NULL,
  `Revision_Date` DATE NOT NULL,
  `Description` TEXT NOT NULL,
  `Longtitude` DOUBLE NOT NULL,
  `Latitude` DOUBLE NOT NULL,
  `Police officer_ID_Badge` VARCHAR(45) NOT NULL,
  `Street_Details_DB_ID` INT NOT NULL,
  INDEX `fk_Report_Police officer1_idx` (`Police officer_ID_Badge` ASC),
  INDEX `fk_Report_Street_Details_DB1_idx` (`Street_Details_DB_ID` ASC),
  PRIMARY KEY (`DOTA`),
  UNIQUE INDEX `Reportcol_UNIQUE` (`DOTA` ASC),
  CONSTRAINT `fk_Report_Police officer1`
    FOREIGN KEY (`Police officer_ID_Badge`)
    REFERENCES `Car_Accidents_DB`.`Police officer` (`ID_Badge`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Report_Street_Details_DB1`
    FOREIGN KEY (`Street_Details_DB_ID`)
    REFERENCES `Car_Accidents_DB`.`Street_Details_DB` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`ADT`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`ADT` (
  `ID_Number` INT NOT NULL,
  `First_Name` VARCHAR(45) NOT NULL,
  `Last_Name` VARCHAR(45) NOT NULL,
  `Address` VARCHAR(45) NOT NULL,
  `Phone Number` VARCHAR(45) NOT NULL,
  `Birth_Date` VARCHAR(45) NOT NULL,
  `AFM` VARCHAR(45) NOT NULL,
  `AMKA` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`ID_Number`),
  UNIQUE INDEX `ADT_UNIQUE` (`ID_Number` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Vehicle_License`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Vehicle_License` (
  `Plate Number` INT NOT NULL,
  `Frame Number` VARCHAR(45) NOT NULL,
  `Brand` VARCHAR(45) NOT NULL,
  `Color` VARCHAR(45) NOT NULL,
  `Cubism` VARCHAR(45) NOT NULL,
  `First_Year` VARCHAR(45) NOT NULL,
  `ADT_ID_Number` INT NOT NULL,
  PRIMARY KEY (`Plate Number`),
  UNIQUE INDEX `idCars_UNIQUE` (`Plate Number` ASC),
  INDEX `fk_DrivingLicense_ID1_idx` (`ADT_ID_Number` ASC),
  CONSTRAINT `fk_DrivingLicense_ID1`
    FOREIGN KEY (`ADT_ID_Number`)
    REFERENCES `Car_Accidents_DB`.`ADT` (`ID_Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Company`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Company` (
  `Code` VARCHAR(45) NOT NULL,
  `Name` VARCHAR(45) NOT NULL,
  `Address` VARCHAR(45) NOT NULL,
  `Phone_Number` VARCHAR(45) NOT NULL,
  `Email` VARCHAR(45) NOT NULL,
  `Type` ENUM('Insurance', 'Commercial') NOT NULL,
  `Company_Code` VARCHAR(45) NULL,
  PRIMARY KEY (`Code`),
  UNIQUE INDEX `idCompany_UNIQUE` (`Code` ASC),
  INDEX `fk_Company_Company1_idx` (`Company_Code` ASC),
  CONSTRAINT `fk_Company_Company1`
    FOREIGN KEY (`Company_Code`)
    REFERENCES `Car_Accidents_DB`.`Company` (`Code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Dangerous_Shipment`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Dangerous_Shipment` (
  `ID` VARCHAR(45) NOT NULL,
  `Rank` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idDangerous_Shipment_UNIQUE` (`ID` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Shipment`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Shipment` (
  `Delivery_Number` INT NOT NULL,
  `Type` VARCHAR(45) NOT NULL,
  `Weight` VARCHAR(45) NOT NULL,
  `Company_Code` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Delivery_Number`),
  UNIQUE INDEX `Delivery_Number_UNIQUE` (`Delivery_Number` ASC),
  INDEX `fk_Shipment_Company1_idx` (`Company_Code` ASC),
  CONSTRAINT `fk_Shipment_Company1`
    FOREIGN KEY (`Company_Code`)
    REFERENCES `Car_Accidents_DB`.`Company` (`Code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Vehicle`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Vehicle` (
  `AI Number` INT NOT NULL AUTO_INCREMENT,
  `Report_DOTA` INT NOT NULL,
  `Type` VARCHAR(45) NOT NULL,
  `Estimated_Speed` INT NOT NULL,
  `Passengers_Number` INT NOT NULL,
  `Company_Code` VARCHAR(45) NULL,
  `Dangerous_Shipment_ID` VARCHAR(45) NULL,
  `Shipment_Delivery_Number` INT NULL,
  `Vehicle_License_Plate Number` INT NOT NULL,
  PRIMARY KEY (`AI Number`),
  UNIQUE INDEX `idCar_UNIQUE` (`AI Number` ASC),
  INDEX `fk_Car_Company1_idx` (`Company_Code` ASC),
  INDEX `fk_Car_Dangerous_Shipment1_idx` (`Dangerous_Shipment_ID` ASC),
  INDEX `fk_Car_Shipment1_idx` (`Shipment_Delivery_Number` ASC),
  INDEX `fk_Vehicle_Report1_idx` (`Report_DOTA` ASC),
  INDEX `fk_Vehicle_Vehicle_License1_idx` (`Vehicle_License_Plate Number` ASC),
  CONSTRAINT `fk_Car_Company1`
    FOREIGN KEY (`Company_Code`)
    REFERENCES `Car_Accidents_DB`.`Company` (`Code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Car_Dangerous_Shipment1`
    FOREIGN KEY (`Dangerous_Shipment_ID`)
    REFERENCES `Car_Accidents_DB`.`Dangerous_Shipment` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Car_Shipment1`
    FOREIGN KEY (`Shipment_Delivery_Number`)
    REFERENCES `Car_Accidents_DB`.`Shipment` (`Delivery_Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Vehicle_Report1`
    FOREIGN KEY (`Report_DOTA`)
    REFERENCES `Car_Accidents_DB`.`Report` (`DOTA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Vehicle_Vehicle_License1`
    FOREIGN KEY (`Vehicle_License_Plate Number`)
    REFERENCES `Car_Accidents_DB`.`Vehicle_License` (`Plate Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Driving_License`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Driving_License` (
  `ID` INT NOT NULL,
  `Nationality` VARCHAR(45) NOT NULL,
  `Rank` VARCHAR(45) NOT NULL,
  `Issue_Date` DATE NOT NULL,
  `ADT_ID_Number` INT NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idDriving_License_UNIQUE` (`ID` ASC),
  INDEX `fk_Driving_License_ADT1_idx` (`ADT_ID_Number` ASC),
  CONSTRAINT `fk_Driving_License_ADT1`
    FOREIGN KEY (`ADT_ID_Number`)
    REFERENCES `Car_Accidents_DB`.`ADT` (`ID_Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Person Status`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Person Status` (
  `ID` INT NOT NULL,
  `Description` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idPerson Status_UNIQUE` (`ID` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Alcohol Test`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Alcohol Test` (
  `ID` VARCHAR(45) NOT NULL,
  `Type` VARCHAR(45) NOT NULL,
  `Time` TIME NOT NULL,
  `Result` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `ID_UNIQUE` (`ID` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Hospital`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Hospital` (
  `ID` INT NOT NULL,
  `Name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idHospital_UNIQUE` (`ID` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`People`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`People` (
  `AI Number` INT NOT NULL AUTO_INCREMENT,
  `Age` INT NOT NULL,
  `Seat` VARCHAR(45) NULL,
  `Safety_Measures` VARCHAR(45) NULL,
  `Violation_Code` VARCHAR(45) NULL,
  `Report_DOTA` INT NOT NULL,
  `Person Status_ID` INT NOT NULL,
  `ADT_ID_Number` INT NOT NULL,
  `Vehicle_AI Number` INT NULL,
  `Driving_License_ID` INT NULL,
  `Alcohol Test_ID` VARCHAR(45) NULL,
  `Hospital_ID` INT NULL,
  PRIMARY KEY (`AI Number`),
  UNIQUE INDEX `idPeople_UNIQUE` (`AI Number` ASC),
  INDEX `fk_People_ID1_idx` (`ADT_ID_Number` ASC),
  INDEX `fk_People_Car1_idx` (`Vehicle_AI Number` ASC),
  INDEX `fk_People_Driving_License1_idx` (`Driving_License_ID` ASC),
  INDEX `fk_People_Person Status1_idx` (`Person Status_ID` ASC),
  INDEX `fk_People_Alcohol Test1_idx` (`Alcohol Test_ID` ASC),
  INDEX `fk_People_Hospital1_idx` (`Hospital_ID` ASC),
  INDEX `fk_People_Report1_idx` (`Report_DOTA` ASC),
  CONSTRAINT `fk_People_ID1`
    FOREIGN KEY (`ADT_ID_Number`)
    REFERENCES `Car_Accidents_DB`.`ADT` (`ID_Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_People_Car1`
    FOREIGN KEY (`Vehicle_AI Number`)
    REFERENCES `Car_Accidents_DB`.`Vehicle` (`AI Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_People_Driving_License1`
    FOREIGN KEY (`Driving_License_ID`)
    REFERENCES `Car_Accidents_DB`.`Driving_License` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_People_Person Status1`
    FOREIGN KEY (`Person Status_ID`)
    REFERENCES `Car_Accidents_DB`.`Person Status` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_People_Alcohol Test1`
    FOREIGN KEY (`Alcohol Test_ID`)
    REFERENCES `Car_Accidents_DB`.`Alcohol Test` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_People_Hospital1`
    FOREIGN KEY (`Hospital_ID`)
    REFERENCES `Car_Accidents_DB`.`Hospital` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_People_Report1`
    FOREIGN KEY (`Report_DOTA`)
    REFERENCES `Car_Accidents_DB`.`Report` (`DOTA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Doctor`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Doctor` (
  `ID` INT NOT NULL,
  `License_Number` INT NOT NULL,
  `Specialization` VARCHAR(45) NOT NULL,
  `Hospital_ID` INT NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idDoctor_UNIQUE` (`ID` ASC),
  INDEX `fk_Doctor_Hospital1_idx` (`Hospital_ID` ASC),
  CONSTRAINT `fk_Doctor_Hospital1`
    FOREIGN KEY (`Hospital_ID`)
    REFERENCES `Car_Accidents_DB`.`Hospital` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Medical Activities`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Medical Activities` (
  `ID` INT NOT NULL,
  `Description` VARCHAR(45) NOT NULL,
  `Body Parts` VARCHAR(45) NOT NULL,
  `Doctor_ID` INT NOT NULL,
  PRIMARY KEY (`ID`),
  UNIQUE INDEX `idMedical Activities_UNIQUE` (`ID` ASC),
  INDEX `fk_Medical Activities_Doctor1_idx` (`Doctor_ID` ASC),
  CONSTRAINT `fk_Medical Activities_Doctor1`
    FOREIGN KEY (`Doctor_ID`)
    REFERENCES `Car_Accidents_DB`.`Doctor` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Admision Hospital`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Admision Hospital` (
  `Protocol_Code` INT NOT NULL,
  `Hospital_ID` INT NOT NULL,
  `Report_DOTA` INT NOT NULL,
  `Admision Date_Time` DATETIME NOT NULL,
  `Discharge Date_Time` DATETIME NOT NULL,
  `Severity` VARCHAR(45) NOT NULL,
  `Description` VARCHAR(45) NOT NULL,
  `ADT_ID_Number` INT NOT NULL,
  `Medical Activities_ID` INT NOT NULL,
  `Treatment_Duration` TIME NOT NULL,
  `Treatment_Result` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`Protocol_Code`),
  UNIQUE INDEX `idAdmision Hospital_UNIQUE` (`Protocol_Code` ASC),
  INDEX `fk_Admision Hospital_ID1_idx` (`ADT_ID_Number` ASC),
  INDEX `fk_Admision Hospital_Medical Activities1_idx` (`Medical Activities_ID` ASC),
  INDEX `fk_Admision Hospital_Report1_idx` (`Report_DOTA` ASC),
  INDEX `fk_Admision Hospital_Hospital1_idx` (`Hospital_ID` ASC),
  CONSTRAINT `fk_Admision Hospital_ID1`
    FOREIGN KEY (`ADT_ID_Number`)
    REFERENCES `Car_Accidents_DB`.`ADT` (`ID_Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Admision Hospital_Medical Activities1`
    FOREIGN KEY (`Medical Activities_ID`)
    REFERENCES `Car_Accidents_DB`.`Medical Activities` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Admision Hospital_Report1`
    FOREIGN KEY (`Report_DOTA`)
    REFERENCES `Car_Accidents_DB`.`Report` (`DOTA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Admision Hospital_Hospital1`
    FOREIGN KEY (`Hospital_ID`)
    REFERENCES `Car_Accidents_DB`.`Hospital` (`ID`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `Car_Accidents_DB`.`Insurance Report`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `Car_Accidents_DB`.`Insurance Report` (
  `AI Number` INT NOT NULL AUTO_INCREMENT,
  `Date` DATE NOT NULL,
  `Estimated Damage` VARCHAR(45) NOT NULL,
  `Estimated Payout` INT NOT NULL,
  `Company_Code` VARCHAR(45) NOT NULL,
  `Vehicle_License_Plate Number` INT NOT NULL,
  `Report_DOTA` INT NOT NULL,
  PRIMARY KEY (`AI Number`),
  INDEX `fk_Insurance Report_Car_License1_idx` (`Vehicle_License_Plate Number` ASC),
  UNIQUE INDEX `AI Number_UNIQUE` (`AI Number` ASC),
  INDEX `fk_Insurance Report_Report1_idx` (`Report_DOTA` ASC),
  CONSTRAINT `fk_Insurance Report_Company1`
    FOREIGN KEY (`Company_Code`)
    REFERENCES `Car_Accidents_DB`.`Company` (`Code`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Insurance Report_Car_License1`
    FOREIGN KEY (`Vehicle_License_Plate Number`)
    REFERENCES `Car_Accidents_DB`.`Vehicle_License` (`Plate Number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Insurance Report_Report1`
    FOREIGN KEY (`Report_DOTA`)
    REFERENCES `Car_Accidents_DB`.`Report` (`DOTA`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
