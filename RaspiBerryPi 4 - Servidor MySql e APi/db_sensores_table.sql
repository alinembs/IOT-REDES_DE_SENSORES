-- MySQL Script generated by MySQL Workbench
-- ter 06 jun 2023 10:51:37
-- Model: New Model    Version: 1.0
-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema esp32
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema esp32
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `esp32` DEFAULT CHARACTER SET utf8 ;
USE `esp32` ;

-- -----------------------------------------------------
-- Table `esp32`.`Sensores`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `esp32`.`Sensores` (
  `Data` DATETIME NOT NULL,
  `TemperaturaC` VARCHAR(10) NOT NULL,
  `TemperaturaF` VARCHAR(10) NOT NULL,
  `Umidade` VARCHAR(10) NOT NULL,
  UNIQUE INDEX `Data_UNIQUE` (`Data` ASC) VISIBLE)
  
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
