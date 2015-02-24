newoption
{
    trigger = 'with-tests',
    description = 'Build with unit tests',
}

newoption
{
    trigger = 'with-benchmark',
    description = 'Build with benchmars',
}

solution 'metasystem'
	configurations { 'Release', 'Debug' }
	location 'build'
	
	project 'umof'
		targetname 'umof'
		language 'C++'
		kind 'SharedLib'
		
		files
		{
			'src/**.h',
			'src/**.cpp'
		}
		
		defines { 'UMOF_LIBRARY' }
		
		configuration 'Debug'
			targetdir 'bin/debug'
			defines { '_DEBUG' }
			flags { 'Symbols' }
			
		configuration 'Release'
			targetdir 'bin/release'
			defines { 'NDEBUG' }
			flags { 'Optimize' }
			
		configuration 'gmake'
			buildoptions { '-std=c++11' }

    if _OPTIONS['with-tests'] then
        project 'test'
            targetname 'umof-tests'
            language 'C++'
            kind 'ConsoleApp'
            
            files
            {
                'test/unit/**.h',
                'test/unit/**.cpp'
            }
            
            links { 'umof' }
            includedirs { 'src' }
            
            configuration 'Debug'
				targetdir 'bin/debug'
				defines { '_DEBUG' }
				flags { 'Symbols' }
			
            configuration 'Release'
                targetdir 'bin/release'
                defines { 'NDEBUG' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
            
    end
    
    if _OPTIONS['with-benchmark'] then
        project 'benchmark'
            targetname 'umof-benchmark'
            language 'C++'
            kind 'ConsoleApp'
            
            files
            {
                'test/bench/bench_native.cpp',
				'test/bench/bench_umof.cpp',
				'test/bench/main.cpp'
            }
			
			if os.findlib('camp') then
				printf('Camp found')
				files { 'test/bench/bench_camp.cpp' }
				links { 'camp' }
			end
			
			if os.findlib('cpgf') then
				printf('CPGF found')
				files { 'test/bench/bench_cpgf.cpp' }
				links { 'cpgf' }
			end
			
			if os.findlib('bin/qt5core') then
				qtRoot = os.findlib('bin/qt5core')
				printf('Qt found at ' .. qtRoot)
				
				files
				{
					'test/bench/qt_test.h',
					'test/bench/moc_qt_test.cpp',
					'test/bench/bench_qt.cpp'
				}
				
				includedirs
				{
					qtRoot .. '/include',
					qtRoot .. '/include/QtCore'
				}
				libdirs { qtRoot .. '/lib' }
				links { 'qt5core' }
			end
            
            links { 'umof' }
            includedirs { 'src' }
            
            configuration 'Debug'
				links { 'celerod' }
				targetdir 'bin/debug'
				defines { '_DEBUG' }
				flags { 'Symbols' }
			
            configuration 'Release'
				links { 'celero' }
                targetdir 'bin/release'
                defines { 'NDEBUG' }
                flags { 'Optimize' }
                
            configuration 'gmake'
                buildoptions { '-std=c++11' }
            
    end
    