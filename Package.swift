// swift-tools-version:5.3

import PackageDescription

let package = Package(
    name: "Adaptr",
    products: [
        .library(
            name: "Adaptr",
            targets: ["Adaptr"]),
    ],
    dependencies: [],
    targets: [
        .binaryTarget(name: "Adaptr", path: "Adaptr.xcframework")
    ]
)
