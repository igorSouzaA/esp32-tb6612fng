# Edit following two lines to set component requirements (see docs)
idf_component_register(SRCS "tb6612_dc_example_main.cpp"
					   INCLUDE_DIRS "../../../TB6612FNG"
					   REQUIRES TB6612FNG driver)