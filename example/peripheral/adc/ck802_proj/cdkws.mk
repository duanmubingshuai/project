.PHONY: clean All Project_Title Project_Build

All: Project_Title Project_Build

Project_Title:
	@echo "----------Building project:[ adc - BuildSet ]----------"

Project_Build:
	@make -r -f adc.mk -C  ./ 


clean:
	@echo "----------Cleaning project:[ adc - BuildSet ]----------"

