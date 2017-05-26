var gulp = require('gulp'),
    gutil = require('gulp-util'),
    sass = require('gulp-sass'),
    sourcemaps = require('gulp-sourcemaps'),
    babel = require('gulp-babel'),
    concat = require('gulp-concat'),
    browserify = require('gulp-browserify');

// Paths
var jsSources = [
      'sites/all/themes/custom/rarefew/js/*.js',
      'sites/all/themes/custom/rarefew/js/**/*.js',
      '!sites/all/themes/custom/rarefew/js/min/*.js',
      '!sites/all/themes/custom/rarefew/js/react_src/*.js',
      '!sites/all/themes/custom/rarefew/js/react_src/**/*.js',
      '!sites/all/themes/custom/rarefew/js/react_builds/*.js',
      '!sites/all/themes/custom/rarefew/js/*.min.js'
    ],
    sassSources = ['sites/all/themes/custom/rarefew/sass/*.scss',
      'sites/all/themes/custom/rarefew/sass/**/*.scss',
    ];

gulp.task('sass', function() {
  gulp.src(sassSources)
  .pipe(sourcemaps.init())
  .pipe(sass({outputStyle: 'compressed'}).on('error', sass.logError))
  .pipe(sourcemaps.write('./maps'))
  .pipe(gulp.dest(sass_dest));
});

gulp.task('watch', function() {
  gulp.watch(sassSources, ['sass']);
});


gulp.task('default', ['watch', 'sass']);
